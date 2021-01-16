#pragma once

#include <string>

namespace net
{
    class InputStream;

    class StreamReader
    {
    public:
        StreamReader(InputStream* socket);

        bool readLine(std::string& str);

    private:
        InputStream* _stream;
    };
}
