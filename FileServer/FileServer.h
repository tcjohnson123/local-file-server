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

    class FileServer : public FormDataHandler
    {
    public:
        FileServer();
        virtual ~FileServer();

        void addDataPair(const std::string& name, const std::string& value) override;
        std::unique_ptr<UploadHandler> createUploadHandler(const std::string& id, const std::string& fname) override;

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
        std::map<std::string, std::string> _post;
        UploadedFile _uploadedFile;
    };
}





