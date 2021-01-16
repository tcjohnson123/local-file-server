#include "pch.h"
#include "UrlEncodedFormDataParser.h"
#include "StringUtils.h"
#include "FormDataHandler.h"
#include <map>

net::UrlEncodedFormDataParser::UrlEncodedFormDataParser(FormDataHandler* handler)
	: FormDataParser(handler)
{
}

void net::UrlEncodedFormDataParser::processChars(char* data, int count)
{
	_stream.write(data, (size_t)count);
}

void net::UrlEncodedFormDataParser::endOfStream()
{
	std::map<std::string, std::string> props;
	StringUtils::decodeFormData(_stream.str(), props);
	for (auto const& [key, value] : props)
		_handler->addDataPair(key, value);
}
