#include "SettingsScene.h"

#include "Globals.h"
#include "Colors.h"
#include "HowToPlayScene.h"
#include "SceneManager.h"
#include "CategoryMenuScene.h"
#include "ButtonFactory.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

#pragma mark Initialization

SettingsScene::SettingsScene()
: resetBtn(NULL)
, oldGlobalLevel(NULL)
{
}

SettingsScene::~SettingsScene()
{
}

CCScene* SettingsScene::scene()
{
    CCScene *scene = CCScene::create();
    scene->addChild(SettingsScene::create());
    return scene;
}

bool SettingsScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    auto menu = CCMenu::create();
    addChild(menu);

    menu->addChild(ButtonFactory::create("Music", this, menu_selector(SettingsScene::btnMusicToggle)));
    menu->addChild(ButtonFactory::create("Sound", this, menu_selector(SettingsScene::btnSoundToggle)));
    menu->addChild(ButtonFactory::create("Change Mode", this, menu_selector(SettingsScene::btnHowToPlay)));

    menu->addChild(ButtonFactory::createEmptyButton());
    menu->addChild(ButtonFactory::create("Remove Ads", this, menu_selector(SettingsScene::btnHowToPlay)));
    menu->addChild(ButtonFactory::create("How to play", this, menu_selector(SettingsScene::btnHowToPlay)));

    menu->addChild(ButtonFactory::createEmptyButton());
    resetBtn = ButtonFactory::create("Reset Game", this, menu_selector(SettingsScene::btnReset));
    menu->addChild(resetBtn);

    menu->alignItemsVerticallyWithPadding(MENU_PADDING);

    addChild(ButtonFactory::createHeadline("Settings"));
    addChild(ButtonFactory::createSceneBackButton());
    addChild(ButtonFactory::createStar());

    return true;
}

void SettingsScene::onEnter()
{
    CCLayer::onEnter();
    
    if (oldGlobalLevel) {
        globalLevel = oldGlobalLevel;
        oldGlobalLevel = NULL;
    }
}

void SettingsScene::btnReset(CCObject* sender)
{
    userstate::resetAllLevelModes();
    resetBtn->setEnabled(false);

    CCObject* it = NULL;
    CCARRAY_FOREACH(resetBtn->getChildren(), it) {
        auto rgbaChild = static_cast<CCNodeRGBA*>(it);
        if (rgbaChild) {
            rgbaChild->setOpacity(DISABLED_OPACITY);
        }
    }
}

void SettingsScene::btnHowToPlay()
{
    oldGlobalLevel = globalLevel;
    globalLevel = NULL;

    SceneManager::getInstance().gotoScene(HowToPlayScene::scene());
}

void SettingsScene::btnMusicToggle()
{
    CCLog("MUSIC");
}

void SettingsScene::btnSoundToggle()
{
    CCLog("SOUND");
}
