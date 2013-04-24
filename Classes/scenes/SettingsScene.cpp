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

    menu->addChild(ButtonFactory::create(_("menu.settings", "music")->getCString(), this, menu_selector(SettingsScene::btnMusicToggle)));
    menu->addChild(ButtonFactory::create(_("menu.settings", "sound")->getCString(), this, menu_selector(SettingsScene::btnSoundToggle)));
    menu->addChild(ButtonFactory::create(_("menu.settings", "changemode")->getCString(), this, menu_selector(SettingsScene::btnHowToPlay)));
    
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
    CCApplication::sharedApplication()
        ->openURL("itms-apps://itunes.com/apps/coragames");
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

void SettingsScene::btnMusicToggle()
{
    CCLog("MUSIC");
}

void SettingsScene::btnSoundToggle()
{
    CCLog("SOUND");
}
