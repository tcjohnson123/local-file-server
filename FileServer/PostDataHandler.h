#pragma once

#include "UploadedFile.h"
#include <string>

namespace net
{
    class PostDataHandler
    {
    public:
        virtual void addDataPair(std::string_view name, std::string_view value) = 0;
        virtual std::unique_ptr<std::ofstream> createStreamForUpload(std::string_view elementId, std::string_view fname) = 0;
    };
}
