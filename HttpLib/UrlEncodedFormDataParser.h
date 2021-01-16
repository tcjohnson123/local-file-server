#pragma once

#include "FormDataParser.h"

namespace net
{
    class FormDataHandler;

    class UrlEncodedFormDataParser : public FormDataParser
    {
    public:
        UrlEncodedFormDataParser(FormDataHandler* handler);

        void processChars(char* data, int count) override;
        void endOfStream() override;
    };
}