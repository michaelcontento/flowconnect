//
//  LanguageKey.cpp
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 03.04.13.
//
//

#include "LanguageKey.h"
#include "Helpers.h"
#include "StringHelper.h"

using namespace std;
using namespace Helpers;

int LanguageKey::count = 0;

LanguageKey::LanguageKey(const char *keyName, const char *keyValue)
: name(keyName)
, value(keyValue)
, parameters()
{
}

LanguageKey* LanguageKey::assign(const char *varName, const char *value)
{
    parameters[varName] = string(value);
    return this;
}

LanguageKey* LanguageKey::assign(const char *varName, int value)
{
    char *strValue = new char[25];
    sprintf(strValue, "%d", value);
    assign(varName, strValue);
    safeDeleteArray(strValue);
    return this;
}

LanguageKey* LanguageKey::assign(const char *varName, float value)
{
    char *strValue = new char[25];
    sprintf(strValue, "%.2f", value);
    assign(varName, strValue);
    safeDeleteArray(strValue);
    return this;
}

std::string LanguageKey::get()
{
    string str = value;

    for (map<string, string>::iterator it = parameters.begin(); it != parameters.end(); ++it)
    {
        StringHelper::replace(str, string(string("{") + it->first + string("}")), string(it->second));
    }
    parameters.clear();
    
    return str;
}

const char * LanguageKey::getCString()
{
    return get().c_str();
}