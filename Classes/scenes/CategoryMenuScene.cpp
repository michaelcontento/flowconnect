#include "CategoryMenuScene.h"

#include "SceneManager.h"
#include "LevelMenuScene.h"
#include "LevelLoader.h"
#include "ButtonFactory.h"
#include "Localization.h"
#include "LanguageKey.h"
#include "userstate.h"

using namespace cocos2d;

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
    addChild(menu);

    addChild(ButtonFactory::createSceneBackButton());
    addChild(ButtonFactory::createStar());
    addChild(ButtonFactory::createHeadline(_("menu.category", "headline").get().c_str()));

    return true;
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
}
