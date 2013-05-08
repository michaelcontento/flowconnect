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
#include "AdManager.h"
#include "ChartboostProvider.h"

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
        _("menu.settings", "music").get().c_str(),
        _("menu.settings", "music.on").get().c_str(),
        _("menu.settings", "music.off").get().c_str(),
        this,
        menu_selector(SettingsScene::btnMusicToggle)
    );
    musicButton->setMode(userstate::hasMusic());
    menu->addChild(musicButton);

    auto soundButton = ToggleButton::create(
        _("menu.settings", "sound").get().c_str(),
        _("menu.settings", "sound.on").get().c_str(),
        _("menu.settings", "sound.off").get().c_str(),
        this,
        menu_selector(SettingsScene::btnSoundToggle)
    );
    soundButton->setMode(userstate::hasSounds());
    menu->addChild(soundButton);

    auto modeButton = ToggleButton::create(
        _("menu.settings", "changemode").get().c_str(),
        _("menu.settings", "changemode.on").get().c_str(),
        _("menu.settings", "changemode.off").get().c_str(),
        this,
        menu_selector(SettingsScene::btnChangeMode)
    );
    modeButton->setMode(userstate::isNumberMode());
    menu->addChild(modeButton);

    menu->addChild(ButtonFactory::createEmptyButton());
    menu->addChild(ButtonFactory::create(_("menu.settings", "howto").get().c_str(), this, menu_selector(SettingsScene::btnHowToPlay)));
    menu->addChild(ButtonFactory::create(_("menu.settings", "moregames").get().c_str(), this, menu_selector(SettingsScene::btnMoreGames)));

    auto showAds = userstate::showAds();
    auto resetable = userstate::resetable();
    if (showAds && resetable) {
        menu->setPositionY(menu->getPositionY() - 30);
    }
    if (showAds || resetable) {
        menu->addChild(ButtonFactory::createEmptyButton());
    }
    if (showAds) {
        auto btn = ButtonFactory::create(_("menu.settings", "removeads").get().c_str(), this, menu_selector(SettingsScene::btnRemoveAds));
        btn->setTag(TAG_REMOVE_ADS);
        menu->addChild(btn);
    }
    if (resetable) {
        auto btn = ButtonFactory::create(_("menu.settings", "resetgame").get().c_str(), this, menu_selector(SettingsScene::btnReset));
        btn->setTag(TAG_RESET_BUTTON);
        menu->addChild(btn);
    }

    menu->alignItemsVerticallyWithPadding(MENU_PADDING);

    addChild(ButtonFactory::createHeadline(_("menu.settings", "headline").get().c_str()));
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
        auto gc = Avalon::GameCenter();
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
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");

    for (auto provider : Ads::AdManager::adProviders) {
        auto cb = static_cast<Ads::ChartboostProvider*>(provider);
        if (cb) {
            cb->openAdLink();
            return;
        }
    }
    
    UrlOpener::open("itms-apps://itunes.com/apps/coragames");
}

void SettingsScene::btnRemoveAds()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");

    mode = MODE_REMOVE_ADS;
    AlertView::createAlert(
        _("alert.removeads", "headline").get().c_str(),
        _("alert.removeads", "body").assign("amount", PRICE_REMOVE_ADS).get().c_str(),
        _("alert.removeads", "btn.cancel").get().c_str()
    );
    AlertView::addAlertButton(_("alert.removeads", "btn.ok").get().c_str());
    AlertView::showAlert(this);
}

void SettingsScene::btnReset()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");

    mode = MODE_RESET_GAME;
    AlertView::createAlert(
        _("alert.resetgame", "headline").get().c_str(),
        _("alert.resetgame", "body").get().c_str(),
        _("alert.resetgame", "btn.cancel").get().c_str()
    );
    AlertView::addAlertButton(_("alert.resetgame", "btn.ok").get().c_str());
    AlertView::showAlert(this);
}

void SettingsScene::btnHowToPlay()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");

    oldGlobalLevel = globalLevel;
    globalLevel = NULL;

    SceneManager::getInstance().gotoScene(HowToPlayScene::scene());
}

void SettingsScene::btnMusicToggle(CCObject* sender)
{
    auto flag = static_cast<CCBool*>(sender);
    userstate::enableMusic(flag->getValue());

    auto engine = CocosDenshion::SimpleAudioEngine::sharedEngine();
    if (flag->getValue()) {
        engine->setBackgroundMusicVolume(1);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    } else {
        engine->setBackgroundMusicVolume(0);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    }

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");
}

void SettingsScene::btnSoundToggle(CCObject* sender)
{
    auto flag = static_cast<CCBool*>(sender);
    userstate::enableSounds(flag->getValue());

    auto engine = CocosDenshion::SimpleAudioEngine::sharedEngine();
    if (flag->getValue()) {
        engine->setEffectsVolume(1);
    } else {
        engine->setEffectsVolume(0);
    }

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");
}

void SettingsScene::btnChangeMode(CCObject* sender)
{
    auto flag = static_cast<CCBool*>(sender);
    userstate::setIsNumberMode(flag->getValue());
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");
}
