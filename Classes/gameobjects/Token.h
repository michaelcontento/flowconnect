#ifndef FlowConnect_Token_h
#define FlowConnect_Token_h

#include "Slot.h"

#define TOKEN_DIRECTION_LEFT 'l'
#define TOKEN_DIRECTION_RIGHT 'r'
#define TOKEN_DIRECTION_UP 'u'
#define TOKEN_DIRECTION_DOWN 'd'

namespace token
{
    bool isDirectionToken(const char token)
    {
        if (token == TOKEN_DIRECTION_LEFT) {
            return true;
        } else if (token == TOKEN_DIRECTION_RIGHT) {
            return true;
        } else if (token == TOKEN_DIRECTION_DOWN) {
            return true;
        } else if (token == TOKEN_DIRECTION_UP) {
            return true;
        }

        return false;
    }
}

#endif