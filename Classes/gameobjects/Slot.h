#ifndef __FlowConnect__Slot__
#define __FlowConnect__Slot__

#include "cocos2d.h"

#define SLOT_DIRECTION_LEFT 0
#define SLOT_DIRECTION_RIGHT 1
#define SLOT_DIRECTION_UP 2
#define SLOT_DIRECTION_DOWN 3

#define SLOT_NO_NUMBER -1

class Slot : public cocos2d::CCNode
{
public:
    Slot();
    virtual ~Slot();

    CREATE_FUNC(Slot);
    virtual bool init();

    void removeDirection();
    void setDirection(const int direction);

    void markAsFinal();
    bool isFinal() const;

    void markAsPending();
    bool isPending() const;

    void markAsNextSlot(const bool flag=true);
    bool isNextSlot() const;

    cocos2d::CCSize getSize();

    CC_SYNTHESIZE_READONLY(int, number, Number)
    void setNumber(const int newNumber);

private:
    bool isFinal_;
    bool isNextSlot_;
    cocos2d::CCSprite* background;
    cocos2d::CCSprite* line;
    cocos2d::CCLabelTTF* label;
    cocos2d::CCSprite* labelBackground;
    cocos2d::CCAction* labelPulseAction;

    void hideNumber();
    void showNumber();
};

#endif /* defined(__FlowConnect__Slot__) */