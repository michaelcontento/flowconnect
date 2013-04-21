#include "MenuScene.h"

#include "SceneManager.h"
#include "CategoryMenuScene.h"
#include "ButtonFactory.h"

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

    auto menu = CCMenu::create();
    addChild(menu);

    menu->addChild(ButtonFactory::create("Play", this, menu_selector(MenuScene::btnPlay)));
    menu->addChild(ButtonFactory::create("Time Attack", this, menu_selector(MenuScene::btnPLayTimeAttack)));
    menu->addChild(ButtonFactory::createEmptyButton());
    menu->addChild(ButtonFactory::create("Leaderboard", this, menu_selector(MenuScene::btnLeaderboard)));
    menu->addChild(ButtonFactory::create("Achievements", this, menu_selector(MenuScene::btnAchievements)));
    menu->addChild(ButtonFactory::createEmptyButton());
    menu->addChild(ButtonFactory::create("Shop", this, menu_selector(MenuScene::btnShop)));
    menu->addChild(ButtonFactory::create("Settings", this, menu_selector(MenuScene::btnSettings)));
    menu->alignItemsVerticallyWithPadding(MENU_PADDING);

    return true;
}

void MenuScene::btnPlay()
{
    SceneManager::getInstance().gotoScene(CategoryMenuScene::scene());
}

void MenuScene::btnPLayTimeAttack()
{
}

void MenuScene::btnLeaderboard()
{
}

void MenuScene::btnAchievements()
{
}

void MenuScene::btnShop()
{
}

void MenuScene::btnSettings()
{
}

