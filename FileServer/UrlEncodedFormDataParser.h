#pragma once

#include "FormDataParser.h"

namespace net
{
    class FormDataHandler;

    class UrlEncodedFormDataParser : public FormDataParser
    {
    public:
        UrlEncodedFormDataParser(FormDataHandler* handler);

        void processChar(char ch) override;
        void endOfStream() override;
    };
}