#include "Alert.h"

#include <boost/cast.hpp>
#include "Colors.h"
#include "Globals.h"

using namespace cocos2d;
using boost::polymorphic_downcast;

Alert::Alert()
: headline(NULL)
, body(NULL)
, closable(false)
, spinner(false)
, menu(NULL)
, menuTracked(false)
{
}

void Alert::onExit()
{
    CCNode::onExit();

    CCDirector::sharedDirector()
        ->getTouchDispatcher()
        ->removeDelegate(this);
}

bool Alert::init()
{
    if (!CCSprite::initWithSpriteFrameName("alert/background.png")) {
        return false;
    }

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    setPosition(ccpMult(ccpFromSize(winSize), 0.5f));
    setAnchorPoint(CCPoint(0.5, 0.5));

    CCDirector::sharedDirector()
        ->getTouchDispatcher()
        ->addTargetedDelegate(this, -INT_MAX, true);

    return true;
}

bool Alert::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!menuTracked && menu) {
        menuTracked = menu->ccTouchBegan(pTouch, pEvent);
    }
    return true;
}

void Alert::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (menuTracked) {
        menu->ccTouchMoved(pTouch, pEvent);
    }
}

void Alert::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (closable) {
        const CCPoint touchPos = convertTouchToNodeSpace(pTouch);
        if (getChildByTag(tagClose)->boundingBox().containsPoint(touchPos)) {
            (closeTarget->*closeSelector)(this);
            return;
        }
    }

    if (menuTracked) {
        menuTracked = false;
        menu->ccTouchEnded(pTouch, pEvent);
    }
}

void Alert::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (menuTracked) {
        menuTracked = false;
        menu->ccTouchCancelled(pTouch, pEvent);
    }
}

void Alert::setHeadline(const char* headline)
{
    this->headline = headline;
    
    auto old = getChildByTag(tagHeadline);
    if (old) {
        removeChild(old, true);
    }

    auto label = CCLabelTTF::create(headline, DEFAULT_FONT_NAME, 36);
    label->setTag(tagHeadline);
    addChild(label);

    updateLayout();
}

void Alert::setBody(const char* body)
{
    this->body = body;

    auto old = getChildByTag(tagBody);
    if (old) {
        removeChild(old, true);
    }

    auto label = CCLabelTTF::create(body, SMALL_FONT_NAME, 28);
    label->setColor(ccGRAY);
    label->setTag(tagBody);
    addChild(label);

    updateLayout();
}

void Alert::setTimeout(const float duration, cocos2d::CCObject* target, cocos2d::SEL_CallFuncN selector)
{
    runAction(
        CCSequence::create(
            CCDelayTime::create(duration),
            CCCallFuncN::create(target, selector),
            NULL
        )
    );
}

void Alert::setCloseAction(cocos2d::CCObject* target, cocos2d::SEL_CallFuncN selector)
{
    closeTarget = target;
    closeSelector = selector;
}

void Alert::setClosable(const bool flag)
{
    this->closable = flag;

    auto old = getChildByTag(tagClose);
    if (old) {
        removeChild(old, true);
    }

    if (!flag) {
        return;
    }

    assert(closeSelector && "you have to set the close action first");

    auto btn = CCSprite::createWithSpriteFrameName("alert/btn-close.png");
    btn->setTag(tagClose);
    addChild(btn);

    auto size = ccpFromSize(getContentSize());
    auto offset = ccpMult(ccpFromSize(btn->getContentSize()), 0.2);
    btn->setPosition(ccpSub(size, offset));
    btn->setAnchorPoint(CCPoint(0.5f, 0.5f));
}

void Alert::showSpinner(const bool flag /* = true */)
{
    spinner = flag;

    auto old = getChildByTag(tagSpinner);
    if (old) {
        removeChild(old, true);
    }

    if (!flag) {
        return;
    }

    auto img = CCSprite::createWithSpriteFrameName("alert/spinner.png");
    img->setTag(tagSpinner);
    img->setScale(1.5);
    addChild(img);

    const float spinnerSteps = 12.f;
    const float spinnerDuration = 0.8f;
    auto action = CCRepeatForever::create(
        CCSequence::create(
            CCDelayTime::create(spinnerDuration / spinnerSteps),
            CCRotateBy::create(0, 360.f / spinnerSteps),
            NULL
        )
    );
    img->runAction(action);

    updateLayout();
}

bool Alert::withSpinner() const
{
    return spinner;
}

void Alert::updateLayout()
{
    auto headlineNode = getChildByTag(tagHeadline);
    auto bodyNode = getChildByTag(tagBody);
    auto menuNode = getChildByTag(tagMenu);
    auto winSize = getContentSize();
    auto spinnerNode = getChildByTag(tagSpinner);

    if (headlineNode) {
        if (bodyNode) {
            headlineNode->setAnchorPoint(CCPoint(0.5, 1));
            headlineNode->setPositionX(winSize.width / 2);
            headlineNode->setPositionY(winSize.height - padding + 5);
        } else {
            headlineNode->setAnchorPoint(CCPoint(0.5, 0.5));
            headlineNode->setPosition(ccpMult(ccpFromSize(winSize), 0.5));
        }
    }

    if (bodyNode) {
        bodyNode->setAnchorPoint(CCPoint(0.5, 0.5));
        bodyNode->setPosition(ccpMult(ccpFromSize(winSize), 0.5));
    }

    if (menuNode) {
        menuNode->setAnchorPoint(CCPoint(0.5, 0));
        menuNode->setPositionX(winSize.width / 2);
        menuNode->setPositionY(padding);

        if (bodyNode) {
            auto basePos = menuNode->getPositionY()
                + (menuNode->getChildrenCount() * (buttonPadding + 40))
                - buttonPadding;
            bodyNode->setPositionY(basePos + (headlineNode->getPositionY() - basePos) / 2);
        }
    }

    if (spinner && spinnerNode) {
        spinnerNode->setAnchorPoint(CCPoint(0.5, 0.5));
        spinnerNode->setPosition(ccpMult(ccpFromSize(winSize), 0.5));
        spinnerNode->setPositionY(
            spinnerNode->getPositionY()
            - spinnerNode->getContentSize().height
            - buttonPadding
        );

        if (bodyNode) {
            auto basePos = spinnerNode->getPositionY();
            bodyNode->setPositionY(basePos + (headlineNode->getPositionY() - basePos) / 2);
        }
    }
}

void Alert::addButton(const char* title, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector)
{
    if (!menu) {
        menu = CCMenu::create();
        menu->setTag(tagMenu);
        addChild(menu);
    }

    auto sprite = CCSprite::createWithSpriteFrameName("alert/btn-choice.png");
    auto btn = CCMenuItemSprite::create(sprite, sprite, target, selector);

    menu->addChild(btn);
    menu->alignItemsVerticallyWithPadding(buttonPadding);
    updateLayout();

    auto label = CCLabelTTF::create(title, DEFAULT_FONT_NAME, 28);
    label->setAnchorPoint(CCPoint(0.5, 0.5));
    label->setPosition(ccpMult(ccpFromSize(btn->getContentSize()), 0.5));
    btn->addChild(label);
}












