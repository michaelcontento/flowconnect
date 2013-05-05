#ifndef __FlowConnect__Alert__
#define __FlowConnect__Alert__

#include "cocos2d.h"

class Alert : public cocos2d::CCSprite, public cocos2d::CCTargetedTouchDelegate
{
public:
    Alert();
    
    CREATE_FUNC(Alert);
    virtual bool init() override;

    void setHeadline(const char* headline);
    void setHeadlineCentered(const char* headline);
    CC_SYNTHESIZE_READONLY(const char*, headline, Headline);

    void setBody(const char* body);
    void setBodyCentered(const char* body);
    CC_SYNTHESIZE_READONLY(const char*, body, Body);

    void setCloseAction(cocos2d::CCObject* target, cocos2d::SEL_CallFuncN selector);
    void setClosable(const bool flag);
    CC_SYNTHESIZE_READONLY(bool, closable, Closable);

    void setTimeout(const float duration, cocos2d::CCObject* target, cocos2d::SEL_CallFuncN selector);

    void addButton(const char* title, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);

    void showSpinner(const bool flag = true);
    bool withSpinner() const;

    virtual void onExit() override;

    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) override;
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) override;
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) override;
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) override;

private:
    const int tagHeadline = 10;
    const int tagBody = 11;
    const int tagClose = 12;
    const int tagMenu = 13;
    const int tagSpinner = 14;
    const int padding = 25;
    const int buttonPadding = 15;
    bool spinner;
    bool menuTracked;
    cocos2d::SEL_CallFuncN closeSelector;
    cocos2d::CCObject* closeTarget;
    cocos2d::CCMenu* menu;

    void updateLayout();
};

#endif /* defined(__FlowConnect__Alert__) */
