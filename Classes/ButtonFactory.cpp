#include "ButtonFactory.h"

#include "SceneManager.h"
#include "Colors.h"

#define BOARD_WIDTH 696

using namespace cocos2d;

unsigned int ButtonFactory::colorCounter = 0;

void ButtonFactory::resetColorCounter()
{
    ButtonFactory::colorCounter = 0;
}

CCMenuItemSprite* ButtonFactory::create(const char* text, CCObject* target, SEL_MenuHandler selector)
{
    auto sprite = CCSprite::createWithSpriteFrameName("buttons/borders/normal.png");
    auto button = CCMenuItemSprite::create(sprite, sprite);
    button->setTarget(target, selector);

    auto label = CCLabelTTF::create(text, "Markler Fett", 48);
    label->setAnchorPoint(CCPoint(0.5, 0.5));
    label->setPosition(ccpMult(ccpFromSize(button->getContentSize()), 0.5));
    label->setColor(LINE_COLORS[ButtonFactory::colorCounter++]);
    button->addChild(label);

    return button;
}

CCLabelTTF* ButtonFactory::createHeadline(const char* text)
{
    auto label = CCLabelTTF::create(text, "Markler Fett", 36);
    label->setAnchorPoint(CCPoint(0, 0.5));
    label->setPositionX(((768 - BOARD_WIDTH) / 2) + 75);
    label->setPositionY((1024 - BOARD_WIDTH) / 4 * 3 + BOARD_WIDTH);
    label->setOpacity(DISABLED_OPACITY);

    return label;
}

CCMenuItemSprite* ButtonFactory::createCategory(LoaderCategory* category, CCObject* target, SEL_MenuHandler selector)
{
    auto sprite = CCSprite::createWithSpriteFrameName("buttons/borders/normal.png");
    auto button = CCMenuItemSprite::create(sprite, sprite);
    button->setTarget(target, selector);

    auto name = CCLabelTTF::create(category->name, "Markler Fett", 36);
    name->setAnchorPoint(CCPoint(0, 1));
    name->setPosition(CCPoint(15, button->getContentSize().height - 7));
    name->setColor(LINE_COLORS[ButtonFactory::colorCounter++]);
    button->addChild(name);

    if (category->description) {
        auto desc = CCLabelTTF::create(category->description, "Markler Fett", 28);
        desc->setAnchorPoint(CCPoint(0, 0));
        desc->setPosition(CCPoint(15, 7));
        desc->setOpacity(DISABLED_OPACITY);
        button->addChild(desc);
    }

    return button;
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

CCMenuItem* ButtonFactory::createEmptyButton()
{
    auto result = CCMenuItem::create();
    result->setContentSize(CCSize(0, 0));
    return result;
}