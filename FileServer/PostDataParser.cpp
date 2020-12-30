#include "pch.h"
#include "PostDataParser.h"

net::PostDataParser::PostDataParser(PostDataHandler* handler)
{
	_handler = handler;
}
