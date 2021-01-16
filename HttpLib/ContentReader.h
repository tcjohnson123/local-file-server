#pragma once

namespace net
{
    class InputStream;

    class ContentChunk
    {
    public:
        char* data;
        int size;

        explicit operator bool() const;
    };

    class ContentReader
    {
    public:
        ContentReader(InputStream* stream, int contentLength, bool contentLengthValid);

        ContentChunk readChunk();

    private:
        InputStream* _stream;
        char _buff[4096];
        int _buffSize;
        int _remaining;
    };
}
