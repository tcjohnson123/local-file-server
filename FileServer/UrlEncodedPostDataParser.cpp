#include "pch.h"
#include "UrlEncodedPostDataParser.h"
#include "StringUtils.h"
#include "FormDataHandler.h"
#include <map>

net::UrlEncodedPostDataParser::UrlEncodedPostDataParser(FormDataHandler* handler)
	: PostDataParser(handler)
{
}

void net::UrlEncodedPostDataParser::processChar(char ch)
{
	_stream << ch;
}

void net::UrlEncodedPostDataParser::endOfStream()
{
	std::map<std::string, std::string> props;
	StringUtils::decodeFormData(_stream.str(), props);
	for (auto const& [key, value] : props)
		_handler->addDataPair(key, value);
}
