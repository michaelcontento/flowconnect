#include "CategoryMenuScene.h"

#include <avalon/i18n/LanguageKey.h>
#include <avalon/i18n/Localization.h>
#include <avalon/ads/Manager.h>
#include "SceneManager.h"
#include "LevelMenuScene.h"
#include "LevelLoader.h"
#include "../buttons/ButtonFactory.h"
#include "userstate.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using avalon::i18n::_;

extern LevelLoader globalLevelLoader;
extern LoaderLevel* globalLevel;

#pragma mark Initialization

CategoryMenuScene::CategoryMenuScene()
: menu(NULL)
, lastStars(-1)
{
}

CategoryMenuScene::~CategoryMenuScene()
{
}

CCScene* CategoryMenuScene::scene()
{
    CCScene *scene = CCScene::create();
    scene->addChild(CategoryMenuScene::create());
    return scene;
}

void CategoryMenuScene::onEnter()
{
    CCLayer::onEnter();

    auto stars = userstate::getStarsForCategory(globalLevel->page->category);
    if (stars != lastStars) {
        fillMenu();
    }
}

bool CategoryMenuScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    menu = CCMenu::create();
    if (avalon::ads::Manager::enabled) {
        menu->setScale(0.9);
        menu->setPositionX(menu->getPositionX() - 45);
        menu->setPositionY(menu->getPositionY() - 35);
    } else {
        menu->setPositionY(menu->getPositionY() - 45);
    }
    addChild(menu);

    addChild(ButtonFactory::createSceneBackButton());
    addChild(ButtonFactory::createStar());
    addChild(ButtonFactory::createHeadline(_("menu.category", "headline").get().c_str()));

    setKeypadEnabled(true);
    return true;
}

void CategoryMenuScene::keyBackClicked()
{
    SceneManager::getInstance().popSceneWithSound();
}

void CategoryMenuScene::fillMenu()
{
    menu->removeAllChildren();

    for (auto category : globalLevelLoader.getCategories()) {
        auto button = ButtonFactory::createCategory(category, this, menu_selector(CategoryMenuScene::btnGame));
        button->setUserData(category);
        menu->addChild(button);
    }
    
    menu->alignItemsVerticallyWithPadding(MENU_PADDING);
}

void CategoryMenuScene::btnGame(void* sender)
{
    auto button = static_cast<CCMenuItemFont*>(sender);
    if (!button) {
        return;
    }

    auto category = static_cast<LoaderCategory*>(button->getUserData());
    if (!category) {
        return;
    }

    if (category->pages.size() == 0) {
        return;
    }

    lastStars = userstate::getStarsForCategory(category);
    globalLevel = category->pages.front()->levels.front();
    SceneManager::getInstance().gotoScene(LevelMenuScene::scene());
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");
}
