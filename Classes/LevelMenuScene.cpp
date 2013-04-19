#include "LevelMenuScene.h"

#include "SceneManager.h"
#include "GameScene.h"
#include "ButtonFactory.h"

using namespace cocos2d;

extern LoaderLevel* globalLevel;

#pragma mark Initialization

LevelMenuScene::LevelMenuScene()
{
}

LevelMenuScene::~LevelMenuScene()
{
}

CCScene* LevelMenuScene::scene()
{
    CCScene *scene = CCScene::create();
    scene->addChild(LevelMenuScene::create());
    return scene;
}

bool LevelMenuScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    addChild(createMenuContainer());
    addChild(ButtonFactory::createSceneBackButton());

    return true;
}

CCNode* LevelMenuScene::createMenuContainer()
{
    CCNode* container = CCNode::create();
    container->setAnchorPoint(CCPoint(0, 0));
    
    auto selector = menu_selector(LevelMenuScene::btnGame);
    auto pageCounter = 0;
    
    for (auto page : globalLevel->page->category->pages) {
        auto menu = CCMenu::create();
        
        for (auto level : page->levels) {
            menu->addChild(ButtonFactory::createLevelButton(level, this, selector));
        }
        alignMenu(menu);

        auto menuContainer = CCNode::create();
        menuContainer->setContentSize(menu->getContentSize());
        menuContainer->setAnchorPoint(CCPoint(0.5, 0.5));
        menuContainer->setPosition((768 * pageCounter++) + 384, 512);
        menuContainer->addChild(menu);

        container->addChild(menuContainer);
    }

    return container;
}

void LevelMenuScene::alignMenu(CCMenu* menu) const
{
    auto rows = CCArray::create();
    auto counter = menu->getChildrenCount();

    while (counter > 0) {
        if (counter >= ITEMS_PER_ROW) {
            rows->addObject(CCInteger::create(ITEMS_PER_ROW));
            counter -= ITEMS_PER_ROW;
        } else {
            rows->addObject(CCInteger::create(counter));
            counter = 0;
        }
    }
    
    menu->alignItemsInColumnsWithArray(rows);
}

void LevelMenuScene::btnGame(void* sender) const
{
    auto button = static_cast<CCMenuItemFont*>(sender);
    if (!button) {
        return;
    }

    auto level = static_cast<LoaderLevel*>(button->getUserData());
    if (!level) {
        return;
    }

    globalLevel = level;
    SceneManager::getInstance().gotoScene(GameScene::scene());
}