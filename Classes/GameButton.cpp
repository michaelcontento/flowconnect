#include "GameButton.h"

#include "Globals.h"
#include "userstate.h"
#include "HowToPlayScene.h"
#include "SceneManager.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

#define ZORDER_STAR -1

extern const LoaderLevel* globalLevel;

using namespace cocos2d;

GameButton::GameButton()
: border(NULL)
, star(NULL)
, label(NULL)
, lastState(userstate::Mode::NONE)
, pagelock(NULL)
{
}

GameButton::~GameButton()
{
}

GameButton* GameButton::createWithLevel(const LoaderLevel* level, PageLockButton* pagelock)
{
    auto result = new GameButton();
    if (result && result->initWithLevel(level, pagelock)) {
        result->autorelease();
        return result;
    } else {
        delete result;
        result = NULL;
        return NULL;
    }
}

bool GameButton::initWithLevel(const LoaderLevel* level, PageLockButton* pagelock)
{
    this->pagelock = pagelock;
    this->level = level;
    if (!level) {
        return false;
    }

    if (!CCMenuItem::initWithTarget(this, menu_selector(GameButton::onClick))) {
        return false;
    }

    addBackground();
    addLabel();
    updateStateIndicator();

    return true;
}

void GameButton::addBackground()
{
    border = CCSprite::createWithSpriteFrameName("buttons/borders/gamemenu.png");
    border->setAnchorPoint(CCPointZero);

    addChild(border);
    setContentSize(border->getContentSize());
}

void GameButton::updateStateIndicator()
{
    auto state = userstate::getModeForLevel(level);
    if (lastState == state) {
        return;
    }

    if (star) {
        removeChild(star);
    }

    star = CCSprite::createWithSpriteFrameName("buttons/borders/star-full.png");
    star->setAnchorPoint(CCPoint(0.5, 0.5));
    star->setPosition(ccpMult(ccpFromSize(getContentSize()), 0.5));
    star->setPositionX(star->getPositionX());
    star->setPositionY(star->getPositionY() + 3);
    star->setZOrder(label->getZOrder() - 1);
    addChild(star);

    if (state == userstate::Mode::SOLVED) {
        star->setOpacity(255 * 0.3);
    } else if (state == userstate::Mode::PERFECT) {
        label->setColor(ccBLACK);
    }
}

void GameButton::addLabel()
{
    static char text[5] = {0};
    snprintf(text, sizeof(text), "%d", level->localid);

    label = CCLabelTTF::create(text, DEFAULT_FONT_NAME, 42);
    label->setAnchorPoint(CCPoint(0.5, 0.5));
    label->setPosition(CCPoint(getContentSize().width / 2, getContentSize().height / 2));
    addChild(label);

    if (level->localid >= 10 && level->localid < 20) {
        label->setPositionX(label->getPositionX() - 2);
    }
}

void GameButton::setBorderColor(const ccColor3B color)
{
    border->setColor(color);
}

void GameButton::onClick()
{
    if (pagelock && pagelock->onClick()) {
        return;
    }

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");
    globalLevel = level;

    if (userstate::showHowToPlay()) {
        userstate::setShowHowToPlay(false);
        SceneManager::getInstance().gotoScene(HowToPlayScene::scene());
    } else {
        SceneManager::getInstance().gotoScene(GameScene::scene());
    }

}
