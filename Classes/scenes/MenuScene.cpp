#include "MenuScene.h"

#include "SceneManager.h"
#include "CategoryMenuScene.h"
#include "ShopScene.h"
#include "ButtonFactory.h"
#include "SettingsScene.h"
#include "Localization.h"
#include "LanguageKey.h"
#include "AdManager.h"
#include "GameCenter.h"

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

    menu->addChild(ButtonFactory::create(_("menu.main", "play")->getCString(), this, menu_selector(MenuScene::btnPlay)));
    //menu->addChild(ButtonFactory::create("Time Attack", this, menu_selector(MenuScene::btnPLayTimeAttack)));
    menu->addChild(ButtonFactory::createEmptyButton());
    menu->addChild(ButtonFactory::create(_("menu.main", "leaderboard")->getCString(), this, menu_selector(MenuScene::btnLeaderboard)));
    menu->addChild(ButtonFactory::create(_("menu.main", "achievements")->getCString(), this, menu_selector(MenuScene::btnAchievements)));
    menu->addChild(ButtonFactory::createEmptyButton());
    menu->addChild(ButtonFactory::create(_("menu.main", "shop")->getCString(), this, menu_selector(MenuScene::btnShop)));
    menu->addChild(ButtonFactory::create(_("menu.main", "settings")->getCString(), this, menu_selector(MenuScene::btnSettings)));
    menu->alignItemsVerticallyWithPadding(MENU_PADDING);

    addChild(ButtonFactory::createStar());

    return true;
}

void MenuScene::btnPlay()
{
    Ads::AdManager::showFullscreenAd();
    SceneManager::getInstance().gotoScene(CategoryMenuScene::scene());
}

void MenuScene::btnPLayTimeAttack()
{
    assert(false && "Not yet implemented");
}

void MenuScene::btnLeaderboard()
{
    auto gc = GameCenter();
    gc.postAchievement("com.coragames.dtdng.ac.rank.baby", 100);
    gc.postAchievement("com.coragames.dtdng.ac.rank.baby.perfect", 75);
    gc.postScore("com.coragames.dtdng.lb.normal", 1234);
    gc.postScore("com.coragames.dtdng.lb.bonus", 12121212);
    gc.openLeaderboards();
}

void MenuScene::btnAchievements()
{
    auto gc = GameCenter();
    gc.openAchievement();
}

void MenuScene::btnShop()
{
    SceneManager::getInstance().gotoScene(ShopScene::scene());
}

void MenuScene::btnSettings()
{
    SceneManager::getInstance().gotoScene(SettingsScene::scene());
}

