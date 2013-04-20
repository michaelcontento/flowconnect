#include "GameButton.h"

#include "SceneManager.h"
#include "GameScene.h"

extern LoaderLevel* globalLevel;

using namespace cocos2d;

GameButton::GameButton()
: border(NULL)
{
}

GameButton::~GameButton()
{
}

GameButton* GameButton::createWithLevel(LoaderLevel* level)
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

bool GameButton::initWithLevel(LoaderLevel* level)
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

    return true;
}

void GameButton::addBackground()
{
    border = CCSprite::createWithSpriteFrameName("buttons/borders/gamemenu.png");
    border->setAnchorPoint(CCPointZero);

    addChild(border);
    setContentSize(border->getContentSize());
}

void GameButton::addLabel()
{
    char text[5] = {0};
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
    SceneManager::getInstance().gotoScene(GameScene::scene());
}
