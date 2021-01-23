#pragma once

#include "RequestHandler.h"
#include "FormDataHandler.h"
#include "UploadedFile.h"
#include <string>
#include <memory>
#include <filesystem>

namespace net
{
    class StreamWriter;
    class UploadHandler;

    class FileServer
    {
    public:
        FileServer(const std::filesystem::path& rootFolder);
        virtual ~FileServer();

        void handleRequest(const HttpRequest& request, bool* keepAlive);

    private:
        static std::string mimeType(const std::filesystem::path& path);
        void serveString(std::string_view status, std::string_view body, const std::list<std::string>& extraHeaders);
        void serveString(std::string_view status, std::string_view body);
        void serve404NotFound();
        void serve401Unauthorized();
        void serveFile(const std::filesystem::path& path);
        void serveDirectory(const std::filesystem::path& path);
        std::string connectionStr() const;

    private:
        std::unique_ptr<StreamWriter> _streamWriter;
        bool* _keepAlive;
        std::filesystem::path _rootFolder;
    };
}





