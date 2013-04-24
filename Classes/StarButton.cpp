#include "StarButton.h"

#include "Colors.h"
#include "Globals.h"
#include "userstate.h"
#include "SceneManager.h"
#include "ShopScene.h"

using namespace cocos2d;

StarButton::StarButton()
: counter(NULL)
, enabled(true)
{
}

StarButton::~StarButton()
{
    globalLastStarButton = NULL;
}

bool StarButton::init()
{
    if (!CCSprite::initWithSpriteFrameName("buttons/star.png")) {
        return false;
    }

    counter = CCLabelTTF::create("", DEFAULT_FONT_NAME, 36);
    counter->setOpacity(DISABLED_OPACITY);
    counter->setAnchorPoint(CCPoint(1, 0.5));
    counter->setPosition(CCPoint(-5, getContentSize().height / 2));
    addChild(counter);

    return true;
}

void StarButton::onEnter()
{
    CCSprite::onEnter();
    globalLastStarButton = this;

    refreshCounter();

    CCDirector::sharedDirector()
        ->getTouchDispatcher()
        ->addTargetedDelegate(this, 1, true);
}

void StarButton::refreshCounter()
{
    char buf[10] = {0};
    snprintf(buf, sizeof(buf), "%d", userstate::getStarsForUser());
    counter->setString(buf);
}

void StarButton::onExit()
{
    CCDirector::sharedDirector()
        ->getTouchDispatcher()
        ->removeDelegate(this);

    CCSprite::onExit();
}

bool StarButton::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    return boundingBox().containsPoint(touch->getLocation());
}

void StarButton::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    if (boundingBox().containsPoint(touch->getLocation())) {
        onClick();
    }
}

void StarButton::onClick()
{
    if (enabled) {
        SceneManager::getInstance().gotoScene(ShopScene::scene());
    }
}