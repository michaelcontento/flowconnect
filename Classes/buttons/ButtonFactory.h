#ifndef __FlowConnect__ButtonFactory__
#define __FlowConnect__ButtonFactory__

#include <avalon/payment/Product.h>
#include "cocos2d.h"
#include "LevelLoader.h"
#include "StarButton.h"

class ButtonFactory
{
public:
    static unsigned int colorCounter;

    static void resetColorCounter();
    static cocos2d::CCMenuItemSprite* create(const char* text, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
    static cocos2d::CCMenu* createSceneBackButton();
    static cocos2d::CCMenuItem* createEmptyButton();
    static cocos2d::CCMenuItemSprite* createCategory(LoaderCategory* category, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
    static cocos2d::CCLabelTTF* createHeadline(const char* text);
    static StarButton* createStar();
    static cocos2d::CCMenuItemSprite* createPaymentButton(avalon::payment::Product* product);
    static cocos2d::CCMenuItemSprite* createPaymentButton(const char* name, const char* desc, const char* price, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
    
private:
    ButtonFactory() {}
    virtual ~ButtonFactory() {}
    DISALLOW_COPY_AND_ASSIGN(ButtonFactory);
};

#endif /* defined(__FlowConnect__ButtonFactory__) */
