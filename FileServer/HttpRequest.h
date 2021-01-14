#pragma once

#include "Stream.h"
#include <string>
#include <list>
#include <map>

namespace net
{
    class PostDataHandler;

    class HttpRequest
    {
    public:
        HttpRequest(Stream* stream, const char* clientIP);
        virtual ~HttpRequest();

        std::string method() const;
        std::string uri() const;
        bool isValid() const;
        std::string clientIP() const;
        Stream* stream() const;
        std::string getHeader(const char* attr, const char* defaultValue = "") const;
        bool hasCredentials() const;
        std::string username() const;
        std::string password() const;
        bool decodeFormData(PostDataHandler* handler) const;
        int contentLength() const;
        bool contentLengthValid() const;
        std::string boundary() const;
        bool hasFormData() const;

    private:
        void addHeader(std::string_view header);
        void parseCredentials();
        void parseContentTypeHeader();
        void parseContentLengthHeader();

    public:
        std::map<std::string, std::string> headers;

    private:
        std::string _method;
        std::string _uri;
        std::string _clientIP;
        Stream* _stream;
        bool _isValid;
        bool _hasCredentials;
        std::string _username;
        std::string _password;
        std::string _boundary;
        int _contentLength;
        std::string _contentType;
        bool _contentLengthValid;
    };
}