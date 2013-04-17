#include "MenuScene.h"

#include "SceneManager.h"
#include "GameScene.h"

using namespace cocos2d;

#pragma mark Initialization

MenuScene::MenuScene()
{
}

MenuScene::~MenuScene()
{
}

CCScene* MenuScene::scene()
{
    CCScene *scene = CCScene::create();
    scene->addChild(MenuScene::create());
    return scene;
}

bool MenuScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    auto game = CCLabelTTF::create("game", "Markler Fett", 24);
    auto gameMenu = CCMenuItemSprite::create(game, game, this, menu_selector(MenuScene::btnGame));

    auto menu = CCMenu::create();
    menu->addChild(gameMenu);
    addChild(menu);

    return true;
}

void MenuScene::btnGame()
{
    SceneManager::getInstance().gotoScene(GameScene::scene());
}