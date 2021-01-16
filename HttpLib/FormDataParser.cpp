#include "pch.h"
#include "FormDataParser.h"

net::FormDataParser::FormDataParser(FormDataHandler* handler)
{
	_handler = handler;
}
