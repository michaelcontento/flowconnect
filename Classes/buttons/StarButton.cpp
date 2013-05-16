#include "StarButton.h"

#include "../Colors.h"
#include "../Globals.h"
#include "../userstate.h"
#include "../scenes/SceneManager.h"
#include "../scenes/ShopScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

StarButton::StarButton()
: counter(NULL)
, enabled(true)
, star(NULL)
, amount(0)
, diffLabel(NULL)
, labelAction(NULL)
, starAction(NULL)
, starEffect(NULL)
{
}

StarButton::~StarButton()
{
    globalLastStarButton = NULL;
    CC_SAFE_RELEASE_NULL(labelAction);
    CC_SAFE_RELEASE_NULL(starAction);
    CC_SAFE_RELEASE_NULL(starEffect);
}

bool StarButton::init()
{
    if (!CCSprite::initWithSpriteFrameName("buttons/border.png")) {
        return false;
    }

    star = CCSprite::createWithSpriteFrameName("buttons/star.png");
    star->setAnchorPoint(CCPoint(0.5, 0.5));
    star->setPosition(ccpMult(ccpFromSize(getContentSize()), 0.5));
    star->setPositionY(star->getPositionY() + 1);
    addChild(star);

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

    refreshCounter(false);

    CCDirector::sharedDirector()
        ->getTouchDispatcher()
        ->addTargetedDelegate(this, 1, true);
}

void StarButton::refreshCounter(const bool withAnimation)
{
    int newAmount = userstate::getStarsForUser();
    int diff = newAmount - amount;
    amount = newAmount;

    if (withAnimation) {
        animate(diff);
    }

    char buf[10] = {0};
    snprintf(buf, sizeof(buf), "%d", newAmount);
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
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");
        SceneManager::getInstance().gotoScene(ShopScene::scene());
    }
}

CCAction* StarButton::getLabelAction()
{
    if (!labelAction) {
        labelAction = CCSequence::create(
            CCFadeIn::create(0.25),
            CCSpawn::create(
                CCFadeOut::create(1.5),
                CCMoveBy::create(2, CCPoint(0, 80)),
                NULL
            ),
            NULL
        );
        labelAction->retain();
    }

    return labelAction;
}

CCAction* StarButton::getStarAction()
{
    if (!starAction) {
        starAction = CCSpawn::create(
            CCSequence::create(
                CCFadeOut::create(1),
                CCFadeIn::create(1),
                NULL
            ),
            NULL
        );
        starAction->retain();
    }

    return starAction;
}

void StarButton::animate(const int diff)
{
    if (diff == 0) {
        return;
    }

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("money.mp3");

    if (!diffLabel) {
        diffLabel = CCLabelTTF::create("", DEFAULT_FONT_NAME, 36);
        diffLabel->setAnchorPoint(CCPoint(1, 0.5));
        addChild(diffLabel);
    }

    char buf[10] = {0};
    if (diff > 0) {
        diffLabel->setColor(ccYELLOW);
        snprintf(buf, sizeof(buf), "+%d", diff);
    } else {
        diffLabel->setColor(ccRED);
        snprintf(buf, sizeof(buf), "%d", diff);
    }
    diffLabel->setString(buf);

    diffLabel->setPosition(counter->getPosition());
    diffLabel->setPositionX(diffLabel->getPositionX() - 15);
    diffLabel->setPositionY(diffLabel->getPositionY() - 25);
    diffLabel->setOpacity(0);
    diffLabel->stopAllActions();
    diffLabel->runAction(getLabelAction());

    if (diff < 0) {
        return;
    }

    if (!starEffect) {
        starEffect = CCParticleSystemQuad::create("star-fx.plist");
        starEffect->setAnchorPoint(star->getAnchorPoint());
        starEffect->setPosition(star->getPosition());
        starEffect->retain();
        addChild(starEffect);
    }
    starEffect->resetSystem();

    star->setOpacity(255);
    star->stopAllActions();
    star->runAction(getStarAction());
}