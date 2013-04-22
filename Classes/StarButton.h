#ifndef __FlowConnect__StarButton__
#define __FlowConnect__StarButton__

#include "cocos2d.h"

class StarButton : public cocos2d::CCSprite, public cocos2d::CCTouchDelegate
{
public:
    StarButton();
    virtual ~StarButton();
    CREATE_FUNC(StarButton);
    bool init();

    void onClick();

    virtual void onEnter();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

private:
    cocos2d::CCLabelTTF* counter;
};

#endif /* defined(__FlowConnect__StarButton__) */
