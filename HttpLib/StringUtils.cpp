#include "pch.h"
#include "StringUtils.h"
#include <sstream>
#include <random>
#include <string.h>

std::string net::StringUtils::toLower(std::string_view stringView)
{
    std::string str(stringView);
    for (auto& ch : str)
        ch = tolower(ch);

    return std::move(str);
}

void net::StringUtils::parseNameValuePairs(std::list<Property>& props, std::string_view str, char delim)
{
    props.clear();
    std::ostringstream name;
    std::ostringstream value;
    int state = 0;
    for (auto ch : str)
    {
        if (state == 0)
        {
            if (ch != ' ')
            {
                name << ch;
                state = 1;
            }
        }
        else if (state == 1)
        {
            if (ch == '=')
            {
                state = 2;
            }
            else if (ch == delim)
            {
                props.push_back(Property(name.str(), value.str()));
                name.str("");
                value.str("");
                state = 0;
            }
            else
            {
                name << ch;
            }
        }
        else if (state == 2)
        {
            if (ch == delim)
            {
                props.push_back(Property(name.str(), value.str()));
                name.str("");
                value.str("");
                state = 0;
            }
            else
            {
                if (ch != '\"')
                    value << ch;
            }
        }
    }
    if (name.str() != "")
    {
        props.push_back(Property(name.str(), value.str()));
    }
}

static int hex2dec(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    else if (ch >= 'A' && ch <= 'F')
        return 10 + ch - 'A';
    else if (ch >= 'a' && ch <= 'f')
        return 10 + ch - 'a';
    return 0;
}

std::string net::StringUtils::urlDecode(std::string_view url)
{
    size_t n = url.size();
    std::ostringstream result;

    for (size_t i = 0; i < n; i++)
    {
        char ch = url[i];
        if ((ch == '%') && (i < n - 2))
        {
            int hiNibble = hex2dec(url[++i]);
            int loNibble = hex2dec(url[++i]);
            result << (char)(16 * hiNibble + loNibble);
        }
        else
        {
            result << ch;
        }
    }
    return result.str();
}

void net::StringUtils::decodeFormData(std::string_view url, std::map<std::string,std::string>& properties)
{
    int state = 0;
    std::ostringstream key;
    std::ostringstream value;
    for (auto ch : url)
    {
        if (ch == '+')
            ch = ' ';
        if (state == 0)
        {
            if (ch == '=')
                state = 1;
            else
                key << ch;
        }
        else if (state == 1)
        {
            if (ch == '&')
            {
                properties[key.str()] = urlDecode(value.str());
                state = 0;
                key.str("");
                value.str("");
            }
            else
            {
                value << ch;
            }
        }
    }
    if (key.str().length() > 0)
        properties[key.str()] = urlDecode(value.str());
}

std::string_view net::StringUtils::ltrim(std::string_view str)
{
    size_t i, n = str.length();
    for (i = 0; i < n && std::isspace(str[i]); i++);

    str.remove_prefix(i);
    return str;
}

std::string_view net::StringUtils::rtrim(std::string_view str)
{
    size_t i, n = str.length();
    for (i = 0; i < n && std::isspace(str[n-i-1]); i++);

    str.remove_suffix(i);
    return str;
}

std::string_view net::StringUtils::trim(std::string_view str)
{
    return rtrim(ltrim(str));
}

class BitStream
{
public:
    std::stringstream bytes;

private:
    uint16_t accumulator;
    int bits = 0;

public:
    void addBits(uint8_t value, int n)
    {
        accumulator = (accumulator << n) + value;
        bits += n;
        if (bits >= 8)
        {
            bytes << (char)(accumulator >> (bits - 8));
            accumulator &= 0xff;
            bits -= 8;
        }
    }
};


std::string net::StringUtils::base64_decode(std::string_view msg)
{
    const char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    BitStream stream;
    for (auto ch : msg)
    {
        auto pos = strchr(alphabet, ch);
        if (pos != nullptr)
            stream.addBits((uint8_t)(pos - alphabet), 6);
    }
    return stream.bytes.str();
}

std::vector<std::string_view> net::StringUtils::split(std::string_view str)
{
    std::vector<std::string_view> tokens;
    size_t start = 0, n = str.length();
    for (size_t i = 0; i < n; i++)
    {
        if (str[i] == ' ')
        {
            tokens.push_back(str.substr(start, i - start));
            start = i + 1;
        }
    }
    tokens.push_back(str.substr(start, n));
    return tokens;
}

std::pair<std::string_view, std::string_view> net::StringUtils::splitAtFirstOf(std::string_view str, char delimiter)
{
    auto n = str.length();
    auto pos = str.find_first_of(delimiter);
    if (pos != std::string::npos)
        return std::make_pair(str.substr(0, pos), str.substr(pos + 1, n - 1));
    else
        return std::make_pair(str.substr(0, pos), str.substr(0, 0));
}

std::string net::StringUtils::randomString(int length)
{
    const std::string alphabet = "0123456789ABCDEF";
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, alphabet.size() - 1);
    std::string result(length, ' ');
    for (auto& ch : result)
        ch = alphabet[distribution(generator)];

    return result;
}
