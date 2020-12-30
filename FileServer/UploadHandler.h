#pragma once

#include <cstddef>

namespace net
{
    class UploadHandler
    {
    public:
        virtual bool handleChunk(const char* data, size_t size) = 0;
        virtual void endOfStream() = 0;
    };
}
