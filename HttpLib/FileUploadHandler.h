#pragma once

#include "UploadHandler.h"
#include <fstream>
#include <filesystem>

namespace net
{
    class FileUploadHandler : public UploadHandler
    {
    public:
        FileUploadHandler(std::string_view id, std::string_view fileName);

        void write(char* data, int size) override;
        void endOfStream() override;
        std::filesystem::path tempName() const;

    private:
        std::string _id;
        std::string _fileName;
        std::filesystem::path _tempName;
        std::ofstream _fs;
    };
}

