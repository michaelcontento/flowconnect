#include "ToggleButton.h"

#include "Globals.h"
#include "ButtonFactory.h"
#include "Colors.h"

using namespace cocos2d;

ToggleButton::ToggleButton()
: label(NULL)
, onLabel(NULL)
, offLabel(NULL)
, mode(true)
, target(NULL)
, selector(NULL)
{
}

ToggleButton::~ToggleButton()
{
    CC_SAFE_RELEASE_NULL(onLabel);
    CC_SAFE_RELEASE_NULL(offLabel);
}


ToggleButton* ToggleButton::create(const char* text, const char* onText, const char* offText, CCObject* target, const SEL_MenuHandler selector)
{
    auto result = new ToggleButton();
    if (result && result->init(text, onText, offText, target, selector)) {
        result->autorelease();
        return result;
    } else {
        delete result;
        result = NULL;
        return NULL;
    }
}

bool ToggleButton::init(const char* text, const char* onText, const char* offText, CCObject* target, const SEL_MenuHandler selector)
{
    if (!CCMenuItem::initWithTarget(this, menu_selector(ToggleButton::onClick))) {
        return false;
    }

    this->target = target;
    this->selector = selector;

    addBackground();
    addLabel(text, onText, offText);
    updateStatusLabel();

    return true;
}

void ToggleButton::addBackground()
{
    auto sprite = CCSprite::createWithSpriteFrameName("buttons/borders/normal.png");
    sprite->setAnchorPoint(CCPointZero);

    addChild(sprite);
    setContentSize(sprite->getContentSize());
}

void ToggleButton::addLabel(const char* text, const char* onText, const char* offText)
{
    label = CCLabelTTF::create(text, DEFAULT_FONT_NAME, 48);
    label->setAnchorPoint(CCPoint(0.5, 0.5));
    label->setColor(LINE_COLORS[ButtonFactory::colorCounter++]);
    label->setPosition(ccpMult(ccpFromSize(getContentSize()), 0.5));
    addChild(label);

    onLabel = CCLabelTTF::create(onText, SMALL_FONT_NAME, 36);
    onLabel->setAnchorPoint(CCPoint(1, 0.5));
    onLabel->setOpacity(DISABLED_OPACITY);
    onLabel->setPosition(CCPoint(
        getContentSize().width - 30,
        getContentSize().height / 2
    ));
    onLabel->retain();

    offLabel = CCLabelTTF::create(offText, SMALL_FONT_NAME, 36);
    offLabel->setAnchorPoint(CCPoint(1, 0.5));
    offLabel->setOpacity(DISABLED_OPACITY);
    offLabel->setPosition(CCPoint(
        getContentSize().width - 30,
        getContentSize().height / 2
    ));
    offLabel->retain();
}

void ToggleButton::setMode(const bool flag)
{
    auto changed = (mode != flag);
    mode = flag;

    if (changed) {
        updateStatusLabel();
    }
}

void ToggleButton::onClick()
{
    mode = !mode;
    updateStatusLabel();
    (target->*selector)(CCBool::create(mode));
}

void ToggleButton::updateStatusLabel()
{
    if (mode) {
        removeChild(offLabel);
        addChild(onLabel);
    } else {
        removeChild(onLabel);
        addChild(offLabel);
    }
}