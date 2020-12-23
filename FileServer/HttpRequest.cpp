#include "pch.h"
#include "HttpRequest.h"
#include "StringUtils.h"
#include "StreamReader.h"
#include "Property.h"
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

net::HttpRequest::HttpRequest(Stream* stream, const char* clientIP)
{
    _isValid = true;
    _hasCredentials = false;
    _clientIP = clientIP;
    _stream = stream;
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
