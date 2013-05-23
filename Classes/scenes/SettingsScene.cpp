#include "SettingsScene.h"

#include <avalon/i18n/LanguageKey.h>
#include <avalon/i18n/Localization.h>
#include <avalon/ads/Manager.h>
#include <avalon/ads/provider/Chartboost.h>
#include <avalon/GameCenter.h>
#include <avalon/utils/url.h>
#include <avalon/utils/platform.h>
#include <avalon/ui/Alert.h>
#include "Globals.h"
#include "Colors.h"
#include "HowToPlayScene.h"
#include "SceneManager.h"
#include "CategoryMenuScene.h"
#include "../buttons/ButtonFactory.h"
#include "SimpleAudioEngine.h"
#include "userstate.h"
#include "../buttons/ToggleButton.h"

using namespace cocos2d;
using namespace CocosDenshion;
using avalon::i18n::_;

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

    setKeypadEnabled(true);
    return true;
}

void SettingsScene::keyBackClicked()
{
    SceneManager::getInstance().popSceneWithSound();
}

void SettingsScene::onAlertButtonClick(const unsigned int index, const std::string title)
{
    if (index == 0) {
        return;
    }

    if (mode == MODE_RESET_GAME) {
        userstate::forceRefillFreeHints();
        userstate::setHintWarning(true);
        userstate::resetAllLevelModes();
        auto gc = avalon::GameCenter();
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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    for (auto provider : avalon::ads::Manager::adProviders) {
        auto cb = static_cast<avalon::ads::provider::Chartboost*>(provider);
        if (cb) {
            cb->openAdLink();
            return;
        }
    }

    avalon::utils::url::open("itms-apps://itunes.com/apps/coragames");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #if (AVALON_PLATFORM_FLAVOR == AVALON_PLATFORM_FLAVOR_AMAZON)
        avalon::utils::url::open("amzn://apps/android?s=CoRa%20Games");
    #elif (AVALON_PLATFORM_FLAVOR == AVALON_PLATFORM_FLAVOR_GOOGLE)
        avalon::utils::url::open("market://search?q=pub:CoRa++Games");
    #elif (AVALON_PLATFORM_FLAVOR == AVALON_PLATFORM_FLAVOR_SAMSUNG)
        avalon::utils::url::open("samsungapps://SellerDetail/fnvddticys");
    #endif
#endif
}

void SettingsScene::btnRemoveAds()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");

    mode = MODE_REMOVE_ADS;
    avalon::ui::Alert alert(this);
    alert.setTitle(_("alert.removeads", "headline").get().c_str());
    alert.setMessage(_("alert.removeads", "body").assign("amount", PRICE_REMOVE_ADS).get().c_str());
    alert.addButton(0, _("alert.removeads", "btn.cancel").get().c_str());
    alert.addButton(1, _("alert.removeads", "btn.ok").get().c_str());
    alert.show();
}

void SettingsScene::btnReset()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");

    mode = MODE_RESET_GAME;
    avalon::ui::Alert alert(this);
    alert.setTitle(_("alert.resetgame", "headline").get().c_str());
    alert.setMessage(_("alert.resetgame", "body").get().c_str());
    alert.addButton(0, _("alert.resetgame", "btn.cancel").get().c_str());
    alert.addButton(1, _("alert.resetgame", "btn.ok").get().c_str());
    alert.show();
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
        engine->playBackgroundMusic("background.mp3", true);
    } else {
        engine->stopBackgroundMusic(false);
    }
}

void SettingsScene::btnSoundToggle(CCObject* sender)
{
    auto flag = static_cast<CCBool*>(sender);
    userstate::enableSounds(flag->getValue());

    auto engine = CocosDenshion::SimpleAudioEngine::sharedEngine();
    if (flag->getValue()) {
        engine->setEffectsVolume(1);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");
    } else {
        engine->setEffectsVolume(0);
    }
}

void SettingsScene::btnChangeMode(CCObject* sender)
{
    auto flag = static_cast<CCBool*>(sender);
    userstate::setIsNumberMode(flag->getValue());
}
