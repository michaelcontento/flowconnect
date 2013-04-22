#include "GameButton.h"

#include "userstate.h"
#include "HowToPlayScene.h"
#include "SceneManager.h"
#include "GameScene.h"

#define ZORDER_STAR -1

extern const LoaderLevel* globalLevel;

using namespace cocos2d;

GameButton::GameButton()
: border(NULL)
, star(NULL)
, lastState(userstate::Mode::NONE)
{
}

GameButton::~GameButton()
{
}

GameButton* GameButton::createWithLevel(const LoaderLevel* level)
{
    auto result = new GameButton();
    if (result && result->initWithLevel(level)) {
        result->autorelease();
        return result;
    } else {
        delete result;
        result = NULL;
        return NULL;
    }
}

bool GameButton::initWithLevel(const LoaderLevel* level)
{
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

    if (state == userstate::Mode::SOLVED) {
        star = CCSprite::createWithSpriteFrameName("buttons/borders/star-empty.png");
    } else if (state == userstate::Mode::PERFECT) {
        star = CCSprite::createWithSpriteFrameName("buttons/borders/star-full.png");
    }

    star->setAnchorPoint(CCPoint(0.5, 0.5));
    star->setOpacity(255 * 0.3);
    star->setPosition(ccpMult(ccpFromSize(getContentSize()), 0.5));
    star->setPositionX(star->getPositionX() + 1);
    star->setZOrder(ZORDER_STAR);
    addChild(star);
}

void GameButton::addLabel()
{
    static char text[5] = {0};
    snprintf(text, sizeof(text), "%d", level->localid);

    auto label = CCLabelTTF::create(text, "Markler Fett", 48);
    label->setAnchorPoint(CCPoint(0.5, 0.5));
    label->setPosition(CCPoint(getContentSize().width / 2, getContentSize().height / 2));

    addChild(label);
}

void GameButton::setBorderColor(const ccColor3B color)
{
    border->setColor(color);
}

void GameButton::onClick()
{
    globalLevel = level;

    if (userstate::showHowToPlay()) {
        userstate::setShowHowToPlay(false);
        SceneManager::getInstance().gotoScene(HowToPlayScene::scene());
    } else {
        SceneManager::getInstance().gotoScene(GameScene::scene());
    }

}
