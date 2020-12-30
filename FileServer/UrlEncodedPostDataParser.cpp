#include "pch.h"
#include "UrlEncodedPostDataParser.h"

net::UrlEncodedPostDataParser::UrlEncodedPostDataParser(PostDataHandler* handler)
	: PostDataParser(handler)
{
}

void net::UrlEncodedPostDataParser::processChar(char ch)
{
}

void net::UrlEncodedPostDataParser::endOfStream()
{
}
