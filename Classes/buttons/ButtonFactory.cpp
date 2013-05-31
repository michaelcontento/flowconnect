#include "ButtonFactory.h"

#include <avalon/utils/platform.h>
#include <avalon/GameCenter.h>
#include <avalon/i18n/LanguageKey.h>
#include <avalon/i18n/Localization.h>
#include "../Globals.h"
#include "../scenes/SceneManager.h"
#include "../Colors.h"
#include "../buttons/StarButton.h"

#define BOARD_WIDTH 696
#define TOP_MARGIN (1024 - 50)
#define SIDE_MARGIN ((768 - BOARD_WIDTH) / 2)

using namespace cocos2d;
using avalon::i18n::_;

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

        auto solved = CCLabelBMFont::create(buf, SMALL_FNT_36, kCCLabelAutomaticWidth, kCCTextAlignmentCenter);
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

CCMenuItemSprite* ButtonFactory::createPaymentButton(avalon::payment::Product* product)
{
    return createPaymentButton(
        product->localizedName.c_str(),
        product->localizedDescription.c_str(),
        product->localizedPrice.c_str(),
        (CCObject*)product, menu_selector(avalon::payment::Product::purchase)
    );
}

CCMenuItemSprite* ButtonFactory::createPaymentButton(const char* name, const char* desc, const char* price, CCObject* target, SEL_MenuHandler selector)
{
    auto sprite = CCSprite::createWithSpriteFrameName("buttons/borders/normal.png");
    auto button = CCMenuItemSprite::create(sprite, sprite);
    button->retain();
    button->setTarget(target, selector);

    auto nameLabel = CCLabelTTF::create(name, DEFAULT_FONT_NAME, 36);
    nameLabel->setAnchorPoint(CCPoint(0, 1));
    nameLabel->setPosition(CCPoint(35, button->getContentSize().height - 7));
    nameLabel->setColor(LINE_COLORS[ButtonFactory::colorCounter++]);
    button->addChild(nameLabel);

    auto descLabel = CCLabelTTF::create(desc, SMALL_FONT_NAME, 28);
    descLabel->setAnchorPoint(CCPoint(0, 0));
    descLabel->setPosition(CCPoint(35, 7));
    descLabel->setOpacity(DISABLED_OPACITY);
    button->addChild(descLabel);

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

CCMenuItemSprite* ButtonFactory::createGoogleButton(CCObject* target, SEL_MenuHandler selector)
{
    auto sprite = CCSprite::createWithSpriteFrameName("buttons/borders/normal.png");
    auto button = CCMenuItemSprite::create(sprite, sprite);
    button->retain();
    button->setTarget(target, selector);

    std::string text = _("btn.googlelogin", "signin").get();
#if AVALON_PLATFORM_IS_ANDROID_GOOGLE
    auto gc = avalon::GameCenter();
    if (gc.isSignedIn()) {
        text = _("btn.googlelogin", "signout").get();
    }
#endif

    auto label = CCLabelTTF::create(text.c_str(), DEFAULT_FONT_NAME, 48);
    label->setAnchorPoint(CCPoint(0.5, 0.5));
    label->setPosition(ccpMult(ccpFromSize(button->getContentSize()), 0.5));
    label->setColor(LINE_COLORS[ButtonFactory::colorCounter++]);
    label->setTag(TAG_GOOGLE_LABEL);
    button->addChild(label);

    auto gplus = CCSprite::createWithSpriteFrameName("google/g+.png");
    gplus->setAnchorPoint(CCPoint(0, 0));
    gplus->setPosition(CCPoint(-90, -4));
    label->addChild(gplus);

    return button;
}

CCMenu* ButtonFactory::createSceneBackButton()
{
    auto menu = CCMenu::create();

    auto normal = CCSprite::createWithSpriteFrameName("buttons/home.png");
    menu->addChild(CCMenuItemSprite::create(
        normal, normal, normal,
        &SceneManager::getInstance(), menu_selector(SceneManager::popSceneWithSound)
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
#if AVALON_PLATFORM_IS_IOS || AVALON_PLATFORM_IS_ANDROID_AMAZON || AVALON_PLATFORM_IS_ANDROID_GOOGLE
    result->setContentSize(CCSize(0, 0));
#else
    result->setContentSize(CCSize(0, 25));
#endif
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