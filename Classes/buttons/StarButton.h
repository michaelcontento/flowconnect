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
    void refreshCounter(const bool withAnimation = true);

    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;

private:
    int amount;
    cocos2d::CCLabelTTF* counter;
    cocos2d::CCSprite* star;
    cocos2d::CCLabelTTF* diffLabel;
    cocos2d::CCAction* labelAction;
    cocos2d::CCAction* starAction;
    cocos2d::CCParticleSystemQuad* starEffect;

    cocos2d::CCAction* getLabelAction();
    cocos2d::CCAction* getStarAction();
    void animate(const int diff);
};

#endif /* defined(__FlowConnect__StarButton__) */
