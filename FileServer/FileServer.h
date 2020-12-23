#pragma once

#include "RequestHandler.h"
#include <string>
#include <memory>
#include <filesystem>

namespace net
{
    class StreamWriter;

    class FileServer
    {
    public:
        FileServer();
        virtual ~FileServer();

        void handleRequest(const HttpRequest& request);

    private:
        static std::string mimeType(const std::filesystem::path& path);
        void serveString(std::string_view status, std::string_view body, const std::list<std::string>& extraHeaders);
        void serveString(std::string_view status, std::string_view body);
        void serve404NotFound();
        void serve401Unauthorized();
        void serveFile(const std::filesystem::path& path);
        void serveDirectory(const std::filesystem::path& path);

    private:
        std::unique_ptr<StreamWriter> _streamWriter;
    };
}





