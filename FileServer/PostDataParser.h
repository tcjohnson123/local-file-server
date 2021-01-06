#pragma once

#include <sstream>

namespace net
{
    class PostDataHandler;

    class PostDataParser
    {
    public:
        PostDataParser(PostDataHandler* handler);
        virtual void processChar(char ch) = 0;
        virtual void endOfStream() = 0;

    protected:
        PostDataHandler* _handler;
        std::ostringstream _stream;
    };
}
