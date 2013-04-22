#include "HowToPlayScene.h"

#include "Globals.h"
#include "GameScene.h"
#include "ButtonFactory.h"
#include "SceneManager.h"

using namespace cocos2d;

#pragma mark Initialization

HowToPlayScene::HowToPlayScene()
{
}

HowToPlayScene::~HowToPlayScene()
{
}

CCScene* HowToPlayScene::scene()
{
    CCScene *scene = CCScene::create();
    scene->addChild(HowToPlayScene::create());
    return scene;
}

bool HowToPlayScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    auto menu = CCMenu::create();
    menu->addChild(ButtonFactory::create("Ok", this, menu_selector(HowToPlayScene::btnPlay)));
    addChild(menu);

    addChild(ButtonFactory::createHeadline("How to play"));
    addChild(ButtonFactory::createSceneBackButton());
    addChild(ButtonFactory::createStar());

    return true;
}

void HowToPlayScene::btnPlay()
{
    if (globalLevel) {
        SceneManager::getInstance().gotoScene(GameScene::scene(), false);
    } else {
        SceneManager::getInstance().popScene();
    }
}