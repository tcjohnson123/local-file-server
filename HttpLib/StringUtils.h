#pragma once

#include "Property.h"
#include <string>
#include <list>
#include <map>
#include <filesystem>

namespace net::StringUtils
{
    std::string toLower(std::string_view str);
    void parseNameValuePairs(std::list<Property>& props, std::string_view str, char delim);
    std::string urlDecode(std::string_view url);
    void decodeFormData(std::string_view url, std::map<std::string, std::string>& properties);
    std::string_view ltrim(std::string_view str);
    std::string_view rtrim(std::string_view str);
    std::string_view trim(std::string_view str);
    std::string base64_decode(std::string_view encoded);
    std::vector<std::string_view> split(std::string_view str);
    std::pair<std::string_view, std::string_view> splitAtFirstOf(std::string_view str, char delimiter);
    std::string randomString(int length);
}
