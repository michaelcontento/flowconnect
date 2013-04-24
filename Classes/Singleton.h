#ifndef FlowConnect_Singleton_h
#define FlowConnect_Singleton_h

#include "cocos2d.h"

template <typename T>
class Singleton
{
public:
    static T& getInstance()
    {
        static T oInstance;
        return oInstance;
    }

protected:
    Singleton() {}
    virtual ~Singleton() {}

private:
    DISALLOW_COPY_AND_ASSIGN(Singleton);
};

#endif
