#include "ButtonFactory.h"

#include "Globals.h"
#include "SceneManager.h"
#include "Colors.h"
#include "StarButton.h"

#define BOARD_WIDTH 696
#define TOP_MARGIN (1024 - 50)
#define SIDE_MARGIN ((768 - BOARD_WIDTH) / 2)

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

    auto label = CCLabelTTF::create(text, DEFAULT_FONT_NAME, 48);
    label->setAnchorPoint(CCPoint(0.5, 0.5));
    label->setPosition(ccpMult(ccpFromSize(button->getContentSize()), 0.5));
    label->setColor(LINE_COLORS[ButtonFactory::colorCounter++]);
    button->addChild(label);

    return button;
}

CCLabelTTF* ButtonFactory::createHeadline(const char* text)
{
    auto label = CCLabelTTF::create(text, DEFAULT_FONT_NAME, 36);
    label->setAnchorPoint(CCPoint(0, 0.5));
    label->setPositionX(SIDE_MARGIN + 75);
    label->setPositionY(TOP_MARGIN);
    label->setOpacity(DISABLED_OPACITY);

    return label;
}

CCMenuItemSprite* ButtonFactory::createCategory(LoaderCategory* category, CCObject* target, SEL_MenuHandler selector)
{
    auto sprite = CCSprite::createWithSpriteFrameName("buttons/borders/normal.png");
    auto button = CCMenuItemSprite::create(sprite, sprite);
    button->setTarget(target, selector);

    auto name = CCLabelTTF::create(category->getLocalizedName(), DEFAULT_FONT_NAME, 36);
    name->setAnchorPoint(CCPoint(0, 1));
    name->setPosition(CCPoint(35, button->getContentSize().height - 7));
    name->setColor(LINE_COLORS[ButtonFactory::colorCounter++]);
    button->addChild(name);

    auto desc = category->getLocalizedDescription();
    if (strlen(desc) > 0) {
        auto descLabel = CCLabelTTF::create(desc, SMALL_FONT_NAME, 28);
        descLabel->setAnchorPoint(CCPoint(0, 0));
        descLabel->setPosition(CCPoint(35, 7));
        descLabel->setOpacity(DISABLED_OPACITY);
        button->addChild(descLabel);
    } else {
        name->setAnchorPoint(CCPoint(0, 0.5));
        name->setPositionY(button->getContentSize().height / 2);
    }

    if (category->pages.size() > 0) {
        char buf[10] = {0};
        snprintf(
            buf, sizeof(buf),
            "%d / %d",
            category->countLevelsSolved(),
            category->countLevels()
        );

        auto solved = CCLabelTTF::create(buf, SMALL_FONT_NAME, 36);
        solved->setOpacity(DISABLED_OPACITY);
        solved->setAnchorPoint(CCPoint(1, 0.5));
        solved->setPosition(CCPoint(
            button->getContentSize().width - 30,
            button->getContentSize().height / 2
        ));
        button->addChild(solved);
    }

    return button;
}

CCMenuItemSprite* ButtonFactory::createPaymentButton(Product* product)
{
    auto sprite = CCSprite::createWithSpriteFrameName("buttons/borders/normal.png");
    auto button = CCMenuItemSprite::create(sprite, sprite);
    button->setTarget((CCObject*)product, menu_selector(Product::purchase));

    auto name = product->getLocalizedName().c_str();
    auto nameLabel = CCLabelTTF::create(name, DEFAULT_FONT_NAME, 36);
    nameLabel->setAnchorPoint(CCPoint(0, 1));
    nameLabel->setPosition(CCPoint(35, button->getContentSize().height - 7));
    nameLabel->setColor(LINE_COLORS[ButtonFactory::colorCounter++]);
    button->addChild(nameLabel);

    auto desc = product->getLocalizedDescription().c_str();
    auto descLabel = CCLabelTTF::create(desc, SMALL_FONT_NAME, 28);
    descLabel->setAnchorPoint(CCPoint(0, 0));
    descLabel->setPosition(CCPoint(35, 7));
    descLabel->setOpacity(DISABLED_OPACITY);
    button->addChild(descLabel);

    auto price = product->getLocalizedPrice().c_str();
    auto priceLabel = CCLabelTTF::create(price, SMALL_FONT_NAME, 36);
    priceLabel->setOpacity(DISABLED_OPACITY);
    priceLabel->setAnchorPoint(CCPoint(1, 0.5));
    priceLabel->setPosition(CCPoint(
        button->getContentSize().width - 30,
        button->getContentSize().height / 2
    ));
    button->addChild(priceLabel);
    
    return button;
}

CCMenu* ButtonFactory::createSceneBackButton()
{
    auto menu = CCMenu::create();

    auto normal = CCSprite::createWithSpriteFrameName("buttons/home.png");
    menu->addChild(CCMenuItemSprite::create(
        normal, normal, normal,
        &SceneManager::getInstance(), menu_selector(SceneManager::popScene)
    ));

    menu->setPositionX(SIDE_MARGIN);
    menu->setPositionY(TOP_MARGIN);
    menu->setAnchorPoint(CCPoint(0, 0.5));
    menu->alignItemsHorizontally();

    return menu;
}

CCMenuItem* ButtonFactory::createEmptyButton()
{
    auto result = CCMenuItem::create();
    result->setContentSize(CCSize(0, 0));
    return result;
}

StarButton* ButtonFactory::createStar()
{
    auto star = StarButton::create();
    star->setPositionX(768 - SIDE_MARGIN);
    star->setPositionY(TOP_MARGIN);
    star->setAnchorPoint(CCPoint(1, 0.5));

    return star;
}