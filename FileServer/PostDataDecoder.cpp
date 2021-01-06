#include "pch.h"

#include "PostDataDecoder.h"
#include "MultipartPostDataParser.h"
#include "UrlEncodedPostDataParser.h"
#include "StringUtils.h"
#include "Stream.h"

net::PostDataDecoder::PostDataDecoder(PostDataHandler* handler, const std::string& contentTypeHeader, const std::string& contentLengthHeader)
{
    _handler = handler;
    _contentLengthValid = false;
    parseContentLengthHeader(contentLengthHeader);
    parseContentTypeHeader(contentTypeHeader);
}

bool net::PostDataDecoder::decode(InputStream* stream)
{
    std::unique_ptr<PostDataParser> parser;
    if (_contentType == "multipart/form-data" && _boundary != "")
        parser = std::make_unique<MultipartPostDataParser>(_handler, _boundary);
    else if (_contentType == "application/x-www-form-urlencoded")
        parser = std::make_unique<UrlEncodedPostDataParser>(_handler);
    else
        return false;

    char ch;
    for (int i = 0; i < _contentLength; i++)
    {
        int n = stream->receive(&ch, 1);
        if (n > 0)
            parser->processChar(ch);
        else
            break;
    }
    parser->endOfStream();
    return true;
}

void net::PostDataDecoder::parseContentLengthHeader(const std::string& contentLengthHeader)
{
    if (contentLengthHeader != "")
    {
        _contentLengthValid = true;
        _contentLength = atoi(contentLengthHeader.c_str());
    }
}

void net::PostDataDecoder::parseContentTypeHeader(const std::string& contentTypeHeader)
{
    std::list<Property> props;
    StringUtils::parseNameValuePairs(props, contentTypeHeader.c_str(), ';');
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
