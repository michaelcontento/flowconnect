#ifndef __FlowConnect__StarButton__
#define __FlowConnect__StarButton__

#include "cocos2d.h"

class StarButton : public cocos2d::CCSprite, public cocos2d::CCTouchDelegate
{
public:
    bool enabled;
    
    StarButton();
    virtual ~StarButton();
    CREATE_FUNC(StarButton);
    bool init() override;

    void onClick();

    virtual void onEnter() override;
    virtual void onExit() override;

    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;

private:
    cocos2d::CCLabelTTF* counter;
};

#endif /* defined(__FlowConnect__StarButton__) */
