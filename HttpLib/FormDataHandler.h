#pragma once

#include <string>

namespace net
{
    class UploadHandler;

    class FormDataHandler
    {
    public:
        virtual void addDataPair(const std::string& name, const std::string& value) = 0;
        virtual std::unique_ptr<UploadHandler> createUploadHandler(const std::string& id, const std::string& fname) = 0;
    };
}
