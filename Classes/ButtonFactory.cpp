#include "ButtonFactory.h"

#include "SceneManager.h"
#include "Colors.h"

#define BOARD_WIDTH 696

using namespace cocos2d;

CCMenuItemFont* ButtonFactory::create(const char* text, CCObject* target, SEL_MenuHandler selector)
{
    return CCMenuItemFont::create(text, target, selector);
}

CCMenu* ButtonFactory::createSceneBackButton()
{
    auto menu = CCMenu::create();

    auto normal = CCSprite::createWithSpriteFrameName("buttons/home.png");

    auto selected = CCSprite::createWithSpriteFrameName("buttons/home.png");
    selected->setColor(ccRED);

    auto disabled = CCSprite::createWithSpriteFrameName("buttons/home.png");
    disabled->setOpacity(DISABLED_OPACITY);

    menu->addChild(CCMenuItemSprite::create(
        normal, selected, disabled,
        &SceneManager::getInstance(), menu_selector(SceneManager::popScene)
    ));

    menu->setPositionX((768 - BOARD_WIDTH) / 2);
    menu->setPositionY((1024 - BOARD_WIDTH) / 4 * 3 + BOARD_WIDTH);
    menu->setAnchorPoint(CCPoint(0, 0.5));
    menu->alignItemsHorizontally();

    return menu;
}

GameButton* ButtonFactory::createLevelButton(LoaderLevel* level)
{
    return GameButton::createWithLevel(level);
}