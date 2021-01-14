#pragma once

#include <sstream>

namespace net
{
    class FormDataHandler;

    class PostDataParser
    {
    public:
        PostDataParser(FormDataHandler* handler);
        virtual void processChar(char ch) = 0;
        virtual void endOfStream() = 0;

    protected:
        FormDataHandler* _handler;
        std::ostringstream _stream;
    };
}
