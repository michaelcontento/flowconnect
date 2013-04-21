#include "CategoryMenuScene.h"

#include "SceneManager.h"
#include "LevelMenuScene.h"
#include "LevelLoader.h"
#include "ButtonFactory.h"

using namespace cocos2d;

extern LevelLoader globalLevelLoader;
extern LoaderLevel* globalLevel;

#pragma mark Initialization

CategoryMenuScene::CategoryMenuScene()
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

bool CategoryMenuScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    auto menu = CCMenu::create();
    addChild(menu);

    for (auto category : globalLevelLoader.getCategories()) {
        auto button = ButtonFactory::createCategory(category, this, menu_selector(CategoryMenuScene::btnGame));
        button->setUserData(category);
        menu->addChild(button);
    }
    menu->alignItemsVerticallyWithPadding(MENU_PADDING);

    addChild(ButtonFactory::createSceneBackButton());

    return true;
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

    globalLevel = category->pages.front()->levels.front();
    SceneManager::getInstance().gotoScene(LevelMenuScene::scene());
}