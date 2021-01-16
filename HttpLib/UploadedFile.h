#pragma once

#include <filesystem>

namespace net
{
    class UploadedFile
    {
    public:
        std::filesystem::path tempName;
        std::string fileName;
    };
}
