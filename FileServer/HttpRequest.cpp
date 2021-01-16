#include "pch.h"
#include "HttpRequest.h"
#include "StringUtils.h"
#include "StreamReader.h"
#include "MultipartFormDataParser.h"
#include "UrlEncodedFormDataParser.h"
#include "ContentReader.h"

#include "FormDataHandler.h"
#include "FormData.h"
#include "FileUploadHandler.h"

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

namespace net
{
    class FormDataManager : public FormDataHandler
    {
    public:
        void addDataPair(const std::string& name, const std::string& value) override
        {
            formData.values[std::string(name)] = value;
        }

        std::unique_ptr<UploadHandler> createUploadHandler(const std::string& id, const std::string& fname) override
        {
            auto handler = std::make_unique<net::FileUploadHandler>(id, fname);
            UploadedFile uploadedFile{ handler->tempName(), std::string(fname) };
            formData.files[id] = uploadedFile;
            return handler;
        }

    public:
        FormData formData;
    };
}

net::HttpRequest::HttpRequest(Stream* stream, std::string_view clientIP)
{
    _isValid = true;
    _hasCredentials = false;
    _clientIP = clientIP;
    _stream = stream;
    _contentLength = 0;
    _contentLengthValid = false;
    StreamReader sr(stream);

    std::string str;
    bool rc = sr.readLine(str);
    if (!rc)
    {
        _isValid = false;
        return;
    }
    std::vector<std::string_view> tokens = StringUtils::split(str);

    if (tokens.size() >= 2)
    {
        _method = tokens[0];
        _uri = StringUtils::urlDecode(tokens[1]);
    }

    while (sr.readLine(str))
    {
        if (str.length() > 0)
            addHeader(str);
        else
            break;
    }

    parseCredentials();
    parseContentTypeHeader();
    parseContentLengthHeader();
}

void net::HttpRequest::addHeader(std::string_view header)
{
    auto [name, value] = StringUtils::splitAtFirstOf(header, ':');
    if (!value.empty())
    {
        auto nameTrimmed = StringUtils::toLower(StringUtils::trim(name));
        headers[nameTrimmed] = StringUtils::trim(value);
    }
}

net::HttpRequest::~HttpRequest()
{
}

std::string net::HttpRequest::method() const
{
    return _method;
}

std::string net::HttpRequest::uri() const
{
    return _uri;
}

std::string net::HttpRequest::clientIP() const
{
    return _clientIP;
}

bool net::HttpRequest::isValid() const
{
    return _isValid;
}

std::string net::HttpRequest::getHeader(const char* attr, const char* defaultValue) const
{
    if (headers.count(attr) > 0)
        return headers.at(attr);
    else
        return defaultValue;
}

net::Stream* net::HttpRequest::stream() const
{
    return _stream;
}

void net::HttpRequest::parseCredentials()
{
    std::string auth = getHeader("authorization");
    if (auth != "")
    {
        auto [authType, credentials] = StringUtils::splitAtFirstOf(auth, ' ');
        if (authType == "Basic" && !credentials.empty())
        {
            auto decoded = StringUtils::base64_decode(credentials);
            auto [username, password] = StringUtils::splitAtFirstOf(decoded, ':');
            if (!password.empty())
            {
                _username = username;
                _password = password;
                _hasCredentials = true;
            }
        }
    }
}

bool net::HttpRequest::hasCredentials() const
{
    return _hasCredentials;
}

std::string net::HttpRequest::username() const
{
    return _username;
}

std::string net::HttpRequest::password() const
{
    return _password;
}

bool net::HttpRequest::decodeFormData(FormDataHandler* handler) const
{
    std::unique_ptr<FormDataParser> parser;
    if (_contentType == "multipart/form-data" && _boundary != "")
        parser = std::make_unique<MultipartFormDataParser>(handler, _boundary);
    else if (_contentType == "application/x-www-form-urlencoded")
        parser = std::make_unique<UrlEncodedFormDataParser>(handler);
    else
        return false;

    ContentReader reader(_stream, _contentLength, _contentLengthValid);
    while (auto chunk = reader.readChunk())
        parser->processChars(chunk.data, chunk.size);

    parser->endOfStream();
    return true;
}

std::string net::HttpRequest::readPostData() const
{
    std::ostringstream ss;
    ContentReader reader(_stream, _contentLength, _contentLengthValid);
    while (auto chunk = reader.readChunk())
        ss.write(chunk.data, (size_t)chunk.size);

    return ss.str();
}

net::FormData net::HttpRequest::readFormData() const
{
    FormDataManager formDataManager;
    decodeFormData(&formDataManager);
    return formDataManager.formData;
}

void net::HttpRequest::parseContentTypeHeader()
{
    auto contentLengthHeader = getHeader("content-length");
    if (contentLengthHeader != "")
    {
        _contentLengthValid = true;
        _contentLength = atoi(contentLengthHeader.c_str());
    }
}

void net::HttpRequest::parseContentLengthHeader()
{
    std::list<Property> props;
    auto contentTypeHeader = getHeader("content-type");
    StringUtils::parseNameValuePairs(props, contentTypeHeader, ';');
    if (!props.empty())
    {
        _contentType = props.front().name;
        for (const auto& prop : props)
        {
            if (prop.name == "boundary")
                _boundary = prop.value;
        }
    }
}

int net::HttpRequest::contentLength() const
{
    return _contentLength;
}

bool net::HttpRequest::contentLengthValid() const
{
    return _contentLengthValid;
}

std::string net::HttpRequest::boundary() const
{
    return _boundary;
}

bool net::HttpRequest::hasFormData() const
{
    if (_method != "POST")
        return false;

    return (_contentType == "multipart/form-data" && _boundary != "")
        || (_contentType == "application/x-www-form-urlencoded");
}
