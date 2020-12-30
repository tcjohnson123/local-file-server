#pragma once

#include "UploadHandler.h"
#include <string>
#include <fstream>

namespace net
{
    class UploadFileWriter : public UploadHandler
    {
    public:
        UploadFileWriter(std::string_view fname);

        bool handleChunk(const char* data, size_t size) override;
        void endOfStream() override;

    private:
        std::ofstream _fs;
        //std::filesystem::path _tempName;
    };
}
