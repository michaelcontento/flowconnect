#include "SettingsScene.h"

#include "Globals.h"
#include "Colors.h"
#include "HowToPlayScene.h"
#include "SceneManager.h"
#include "CategoryMenuScene.h"
#include "ButtonFactory.h"
#include "SimpleAudioEngine.h"
#include "Localization.h"
#include "LanguageKey.h"
#include "UrlOpener.h"
#include "userstate.h"
#include "ToggleButton.h"
#include "GameCenter.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define MODE_NONE 0
#define MODE_RESET_GAME 1
#define MODE_REMOVE_ADS 2

#define TAG_RESET_BUTTON 10
#define TAG_REMOVE_ADS 11

#pragma mark Initialization

SettingsScene::SettingsScene()
: oldGlobalLevel(NULL)
, menu(NULL)
, mode(MODE_NONE)
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

    menu = CCMenu::create();
    addChild(menu);

    auto musicButton = ToggleButton::create(
        _("menu.settings", "music")->getCString(),
        _("menu.settings", "music.on")->getCString(),
        _("menu.settings", "music.off")->getCString(),
        this,
        menu_selector(SettingsScene::btnMusicToggle)
    );
    menu->addChild(musicButton);

    auto soundButton = ToggleButton::create(
        _("menu.settings", "sound")->getCString(),
        _("menu.settings", "sound.on")->getCString(),
        _("menu.settings", "sound.off")->getCString(),
        this,
        menu_selector(SettingsScene::btnSoundToggle)
    );
    menu->addChild(soundButton);

    auto modeButton = ToggleButton::create(
        _("menu.settings", "changemode")->getCString(),
        _("menu.settings", "changemode.on")->getCString(),
        _("menu.settings", "changemode.off")->getCString(),
        this,
        menu_selector(SettingsScene::btnChangeMode)
    );
    modeButton->setMode(userstate::isNumberMode());
    menu->addChild(modeButton);

    menu->addChild(ButtonFactory::createEmptyButton());
    menu->addChild(ButtonFactory::create(_("menu.settings", "howto")->getCString(), this, menu_selector(SettingsScene::btnHowToPlay)));
    menu->addChild(ButtonFactory::create(_("menu.settings", "moregames")->getCString(), this, menu_selector(SettingsScene::btnMoreGames)));

    auto showAds = userstate::showAds();
    auto resetable = userstate::resetable();
    if (showAds && resetable) {
        menu->setPositionY(menu->getPositionY() - 30);
    }
    if (showAds || resetable) {
        menu->addChild(ButtonFactory::createEmptyButton());
    }
    if (showAds) {
        auto btn = ButtonFactory::create(_("menu.settings", "removeads")->getCString(), this, menu_selector(SettingsScene::btnRemoveAds));
        btn->setTag(TAG_REMOVE_ADS);
        menu->addChild(btn);
    }
    if (resetable) {
        auto btn = ButtonFactory::create(_("menu.settings", "resetgame")->getCString(), this, menu_selector(SettingsScene::btnReset));
        btn->setTag(TAG_RESET_BUTTON);
        menu->addChild(btn);
    }

    menu->alignItemsVerticallyWithPadding(MENU_PADDING);

    addChild(ButtonFactory::createHeadline(_("menu.settings", "headline")->getCString()));
    addChild(ButtonFactory::createSceneBackButton());
    addChild(ButtonFactory::createStar());

    return true;
}

void SettingsScene::alertViewClickedButtonAtIndex(int buttonIndex)
{
    if (buttonIndex == 0) {
        return;
    }

    if (mode == MODE_RESET_GAME) {
        userstate::forceRefillFreeHints();
        userstate::setHintWarning(true);
        userstate::resetAllLevelModes();
        auto gc = GameCenter();
        gc.clearAllAchievements();
        gc.clearAllScores();

        menu->removeChildByTag(TAG_RESET_BUTTON, true);
        menu->setPositionY(512);
        menu->alignItemsVerticallyWithPadding(MENU_PADDING);
    } else if (mode == MODE_REMOVE_ADS) {
        if (userstate::addStarsToUser(PRICE_REMOVE_ADS * -1)) {
            userstate::setShowAds(false);
            
            menu->removeChildByTag(TAG_REMOVE_ADS, true);
            menu->setPositionY(512);
            menu->alignItemsVerticallyWithPadding(MENU_PADDING);
        }
    }

    mode = MODE_NONE;
}

void SettingsScene::onEnter()
{
    CCLayer::onEnter();
    
    if (oldGlobalLevel) {
        globalLevel = oldGlobalLevel;
        oldGlobalLevel = NULL;
    }
}

void SettingsScene::btnMoreGames()
{
    UrlOpener::open("itms-apps://itunes.com/apps/coragames");
}

void SettingsScene::btnRemoveAds()
{
    char buf[250] = {0};
    snprintf(buf, sizeof(buf), _("alert.removeads", "body")->getCString(), PRICE_REMOVE_ADS);

    mode = MODE_REMOVE_ADS;
    AlertView::createAlert(
        _("alert.removeads", "headline")->getCString(),
        buf,
        _("alert.removeads", "btn.cancel")->getCString()
    );
    AlertView::addAlertButton(_("alert.removeads", "btn.ok")->getCString());
    AlertView::showAlert(this);
}

void SettingsScene::btnReset()
{
    mode = MODE_RESET_GAME;
    AlertView::createAlert(
        _("alert.resetgame", "headline")->getCString(),
        _("alert.resetgame", "body")->getCString(),
        _("alert.resetgame", "btn.cancel")->getCString()
    );
    AlertView::addAlertButton(_("alert.resetgame", "btn.ok")->getCString());
    AlertView::showAlert(this);
}

void SettingsScene::btnHowToPlay()
{
    oldGlobalLevel = globalLevel;
    globalLevel = NULL;

    SceneManager::getInstance().gotoScene(HowToPlayScene::scene());
}

void SettingsScene::btnMusicToggle(CCObject* sender)
{
    auto flag = static_cast<CCBool*>(sender);
    CCLog("MUSIC %s", (flag->getValue()) ? "on" : "off");
}

void SettingsScene::btnSoundToggle(CCObject* sender)
{
    auto flag = static_cast<CCBool*>(sender);
    CCLog("SOUND %s", (flag->getValue()) ? "on" : "off");
}

void SettingsScene::btnChangeMode(CCObject* sender)
{
    auto flag = static_cast<CCBool*>(sender);
    userstate::setIsNumberMode(flag->getValue());
}