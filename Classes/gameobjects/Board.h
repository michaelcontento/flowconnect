#ifndef __FlowConnect__Board__
#define __FlowConnect__Board__

#include "cocos2d.h"
#include <vector>

class Board : public cocos2d::CCLayer
{
public:
    Board();
    virtual ~Board();

    CREATE_FUNC(Board);
    virtual bool init();
    bool initWithLevel(const cocos2d::CCSize newSize, const char* data);

    CC_SYNTHESIZE_READONLY(cocos2d::CCSize, size, Size);

private:
    cocos2d::CCArray* slots;
    std::vector<int> directions;

    void removeAllSlots();
    void createSlotsFromData(const char* data);
    void rearrangeSlots();
};

#endif /* defined(__FlowConnect__Board__) */