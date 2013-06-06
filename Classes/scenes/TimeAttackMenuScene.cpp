#include "TimeAttackMenuScene.h"

#include <vector.h>
#include <list.h>
#include <avalon/i18n/LanguageKey.h>
#include <avalon/i18n/Localization.h>
#include "SceneManager.h"
#include "LevelMenuScene.h"
#include "LevelLoader.h"
#include "../buttons/ButtonFactory.h"
#include "userstate.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using avalon::i18n::_;

extern LevelLoader globalLevelLoader;
extern LoaderLevel* globalLevel;

#pragma mark Initialization

TimeAttackMenuScene::TimeAttackMenuScene()
: menu(NULL)
{
}

TimeAttackMenuScene::~TimeAttackMenuScene()
{
}

CCScene* TimeAttackMenuScene::scene()
{
    CCScene *scene = CCScene::create();
    scene->addChild(TimeAttackMenuScene::create());
    return scene;
}

void TimeAttackMenuScene::onEnter()
{
    CCLayer::onEnter();

    // @TODO: Update records!
}

bool TimeAttackMenuScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    menu = CCMenu::create();
    addChild(menu);
    createMenu();

    addChild(ButtonFactory::createSceneBackButton());
    addChild(ButtonFactory::createStar());
    addChild(ButtonFactory::createHeadline(_("menu.timeattack", "headline").get().c_str()));

    setKeypadEnabled(true);
    return true;
}

void TimeAttackMenuScene::keyBackClicked()
{
    SceneManager::getInstance().popSceneWithSound();
}

void TimeAttackMenuScene::createMenu()
{
    menu->removeAllChildren();

    createButton(0);
    createButton(1);
    createButton(2);
    createButton(3);
    createButton(4);

    // comming soon button
    menu->addChild(ButtonFactory::createCategory(globalLevelLoader.getCategories().back(), NULL, NULL));

    menu->alignItemsVerticallyWithPadding(MENU_PADDING);
}

void TimeAttackMenuScene::createButton(const int id)
{
    char name[20] = {0};
    char desc[20] = {0};
    snprintf(name, sizeof(name), "level.%d.name", id);
    snprintf(desc, sizeof(desc), "level.%d.desc", id);

    auto button = ButtonFactory::createPaymentButton(
        _("menu.timeattack", name).get().c_str(),
        _("menu.timeattack", desc)
        .assign("time.level", (int)BoardStats::getAttackLevelTime(id))
        .assign("time.puffer", (int)BoardStats::getAttackPufferTime(id))
        .get().c_str(),
        "-",
        this, menu_selector(TimeAttackMenuScene::btnGame)
    );
    button->setUserObject(CCInteger::create(id));
    menu->addChild(button);
}

void TimeAttackMenuScene::btnGame(void* sender)
{
    auto button = static_cast<CCMenuItemFont*>(sender);
    if (!button) {
        return;
    }

    auto index = static_cast<CCInteger*>(button->getUserObject());
    if (!index) {
        return;
    }

    GameScene::mode = GameScene::MODE_TIMEATTACK;
    GameScene::timeAttackId = index->getValue();

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");
    SceneManager::getInstance().gotoScene(GameScene::scene());
}
