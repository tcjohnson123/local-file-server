#pragma once

namespace net
{
    class UploadHandler
    {
    public:
        virtual void write(char* data, int size) = 0;
        virtual void endOfStream() = 0;
    };
}
