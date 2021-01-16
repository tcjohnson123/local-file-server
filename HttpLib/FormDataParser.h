#pragma once

#include <sstream>

namespace net
{
    class FormDataHandler;

    class FormDataParser
    {
    public:
        FormDataParser(FormDataHandler* handler);
        virtual void processChars(char* data, int count) = 0;
        virtual void endOfStream() = 0;

    protected:
        FormDataHandler* _handler;
        std::ostringstream _stream;
    };
}
