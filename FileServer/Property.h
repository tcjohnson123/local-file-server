#pragma once

#include <string>

namespace net
{
    class Property
    {
    public:
        Property(const std::string& name_, const std::string& value_)
        {
            name = name_;
            value = value_;
        }

    public:
        std::string name;
        std::string value;
    };
}