#ifndef __FlowConnect__ButtonFactory__
#define __FlowConnect__ButtonFactory__

#include "cocos2d.h"
#include "LevelLoader.h"

class ButtonFactory
{
public:
    static cocos2d::CCMenuItemFont* create(const char* text, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
    static cocos2d::CCMenu* createSceneBackButton();
    static cocos2d::CCMenuItemFont* createLevelButton(LoaderLevel* level, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);

private:
    ButtonFactory() {}
    virtual ~ButtonFactory() {}
    ButtonFactory(const ButtonFactory&);
    ButtonFactory& operator=(const ButtonFactory&) { return *this; };
};

#endif /* defined(__FlowConnect__ButtonFactory__) */
