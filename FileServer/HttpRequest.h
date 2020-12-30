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
        bool decodePostData(PostDataHandler* handler) const;

    private:
        void addHeader(std::string_view header);
        void parseCredentials();

    public:
        std::map<std::string, std::string> headers;

    private:
        std::string _method;
        std::string _uri;
        std::string _clientIP;
        Stream* _stream;
        bool _isValid;
        std::string _boundary;
        bool _hasCredentials;
        std::string _username;
        std::string _password;
    };
}