#include "MenuScene.h"

#include "SceneManager.h"
#include "CategoryMenuScene.h"
#include "ShopScene.h"
#include "ButtonFactory.h"
#include "SettingsScene.h"
#include "Localization.h"
#include "LanguageKey.h"
#include "GameCenter.h"
#include "Loader.h"

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

    menu->addChild(ButtonFactory::create(_("menu.main", "play").get().c_str(), this, menu_selector(MenuScene::btnPlay)));
    //menu->addChild(ButtonFactory::create("Time Attack", this, menu_selector(MenuScene::btnPLayTimeAttack)));
    menu->addChild(ButtonFactory::createEmptyButton());
    menu->addChild(ButtonFactory::create(_("menu.main", "leaderboard").get().c_str(), this, menu_selector(MenuScene::btnLeaderboard)));
    menu->addChild(ButtonFactory::create(_("menu.main", "achievements").get().c_str(), this, menu_selector(MenuScene::btnAchievements)));
    menu->addChild(ButtonFactory::createEmptyButton());
    menu->addChild(ButtonFactory::create(_("menu.main", "shop").get().c_str(), this, menu_selector(MenuScene::btnShop)));
    menu->addChild(ButtonFactory::create(_("menu.main", "settings").get().c_str(), this, menu_selector(MenuScene::btnSettings)));
    menu->alignItemsVerticallyWithPadding(MENU_PADDING);

    addChild(ButtonFactory::createStar());

    return true;
}

void MenuScene::btnPlay()
{
    SceneManager::getInstance().gotoScene(CategoryMenuScene::scene());
}

void MenuScene::btnPLayTimeAttack()
{
    assert(false && "Not yet implemented");
}

void MenuScene::btnLeaderboard()
{
    auto gc = Avalon::GameCenter();
    gc.showScores();
}

void MenuScene::btnAchievements()
{
    auto gc = Avalon::GameCenter();
    gc.showAchievements();
}

void MenuScene::btnShop()
{
    SceneManager::getInstance().gotoScene(ShopScene::scene());
}

void MenuScene::btnSettings()
{
    SceneManager::getInstance().gotoScene(SettingsScene::scene());
}

