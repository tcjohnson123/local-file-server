#pragma once

#include <string>

namespace net
{
    class OutputStream;

    class StreamWriter
    {
    public:
        StreamWriter(OutputStream* socket);

        bool write(const char* buff, int length);
        bool write(std::string_view str);

    private:
        OutputStream* _stream;
    };
}
