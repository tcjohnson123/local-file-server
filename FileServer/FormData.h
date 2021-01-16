#pragma once

#include "UploadedFile.h"
#include <map>
#include <string>

namespace net
{
	class FormData
	{
	public:
		std::map<std::string, std::string> values;
		std::map<std::string, UploadedFile> files;
	};
}