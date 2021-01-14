#pragma once

#include "PostDataParser.h"

namespace net
{
    class FormDataHandler;

    class UrlEncodedPostDataParser : public PostDataParser
    {
    public:
        UrlEncodedPostDataParser(FormDataHandler* handler);

        void processChar(char ch) override;
        void endOfStream() override;
    };
}