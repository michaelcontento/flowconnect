#ifndef __FlowConnect__ButtonFactory__
#define __FlowConnect__ButtonFactory__

#include "cocos2d.h"
#include "LevelLoader.h"
#include "GameButton.h"

class ButtonFactory
{
public:
    static void resetColorCounter();
    static cocos2d::CCMenuItemSprite* create(const char* text, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
    static cocos2d::CCMenu* createSceneBackButton();
    static GameButton* createLevelButton(LoaderLevel* level);
    static cocos2d::CCMenuItem* createEmptyButton();
    static cocos2d::CCMenuItemSprite* createCategory(LoaderCategory* category, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
    static cocos2d::CCLabelTTF* createHeadline(const char* text);

private:
    static unsigned int colorCounter;
    
    ButtonFactory() {}
    virtual ~ButtonFactory() {}
    ButtonFactory(const ButtonFactory&);
    ButtonFactory& operator=(const ButtonFactory&) { return *this; };
};

#endif /* defined(__FlowConnect__ButtonFactory__) */
