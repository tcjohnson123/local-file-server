#pragma once

#include <sstream>

namespace net
{
    class FormDataHandler;

    class FormDataParser
    {
    public:
        FormDataParser(FormDataHandler* handler);
        virtual void processChar(char ch) = 0;
        virtual void endOfStream() = 0;

    protected:
        FormDataHandler* _handler;
        std::ostringstream _stream;
    };
}
