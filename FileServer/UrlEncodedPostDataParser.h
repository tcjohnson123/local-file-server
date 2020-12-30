#pragma once

#include "PostDataParser.h"

namespace net
{
    class PostDataHandler;

    class UrlEncodedPostDataParser : public PostDataParser
    {
    public:
        UrlEncodedPostDataParser(PostDataHandler* handler);

        void processChar(char ch) override;
        void endOfStream() override;
    };
}