//
//  LanguageKey.cpp
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 03.04.13.
//
//

#include "LanguageKey.h"

#include <boost/algorithm/string/replace.hpp>

int LanguageKey::count = 0;

LanguageKey::LanguageKey(const char* keyName, const char* keyValue)
: name(keyName)
, value(keyValue)
, parameters()
{
    assert(!name.empty() && "name can't be empty");
}

LanguageKey& LanguageKey::assign(const char* varName, const char* value)
{
    parameters[std::string("{").append(varName).append("}")] = std::string(value);
    return *this;
}

LanguageKey& LanguageKey::assign(const char* varName, int value)
{
    char buffer[25] = {0};
    snprintf(buffer, sizeof(buffer), "%d", value);
    assign(varName, buffer);
    
    return *this;
}

LanguageKey& LanguageKey::assign(const char* varName, float value)
{
    return assign(varName, value, "%.2f");
}

LanguageKey& LanguageKey::assign(const char* varName, float value, const char* format)
{
    assert(format && "no format given");
    
    char buffer[25] = {0};
    snprintf(buffer, sizeof(buffer), format, value);
    assign(varName, buffer);
    
    return *this;
}

std::string LanguageKey::get()
{
    if (parameters.empty()) {
        return value;
    }

    std::string formatted = value;

    for (auto row : parameters) {
        boost::replace_all(formatted, row.first, row.second);
    }
    parameters.clear();

    return formatted;
}