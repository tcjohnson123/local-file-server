#pragma once

#include <string>

namespace net
{
    class UploadHandler;

    class FormDataHandler
    {
    public:
        virtual void addDataPair(std::string_view name, std::string_view value) = 0;
        virtual std::unique_ptr<UploadHandler> createUploadHandler(std::string_view elementId, std::string_view fname) = 0;
    };
}
