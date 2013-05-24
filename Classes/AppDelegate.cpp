#include "AppDelegate.h"

#include <avalon/payment.h>
#include <avalon/ads/Manager.h>
#include <avalon/GameCenter.h>
#include <avalon/i18n/Localization.h>
#include "scenes/SceneManager.h"
#include "scenes/MenuScene.h"
#include "SimpleAudioEngine.h"
#include "Globals.h"
#include "userstate.h"
#include "AssetsManager.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace avalon;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

#pragma mark Lifecycle

bool AppDelegate::applicationDidFinishLaunching()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    //pDirector->setDisplayStats(true);

    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(pEGLView);
    pEGLView->setDesignResolutionSize(768, 1024, kResolutionExactFit);

    
    CCSpriteFrameCache::sharedSpriteFrameCache()
        ->addSpriteFramesWithFile("assets.plist");

    initPayment();
    initAds();
    initLocalization();

    auto gc = avalon::GameCenter();
    gc.login();

    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("background.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("checkpoint-1.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("checkpoint-2.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("checkpoint-3.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("click.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("not-solved.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("solved.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("money.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.15);
    if (userstate::hasMusic()) {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background.mp3", true);
    }
    if (!userstate::hasSounds()) {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
    }

    userstate::refreshFreeHints();
    SceneManager::getInstance().gotoScene(MenuScene::scene());

    globalLevelLoader.loadLevel("levels");
    globalLevel = globalLevelLoader.getCategories().front()->pages.front()->levels.front();

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseAllEffects();

    if (userstate::hasMusic()) {
        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    }
}

void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    if (userstate::hasMusic()) {
        SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    }
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();

    userstate::refreshFreeHints();

    if (userstate::showAds()) {
        avalon::ads::Manager::showFullscreenAd();
    }
}

void AppDelegate::initPayment()
{
    payment::Loader loader("payment.ini");
    payment::Loader::globalManager = loader.getManager();
}

void AppDelegate::initAds()
{
    auto mgr = new cocos2d::extension::AssetsManager(
        "http://appdata.coragames.com/dtdng/package.zip",
        "http://appdata.coragames.com/dtdng/version"
    );
    mgr->update();

    avalon::ads::Manager::initWithIniFile("ads.ini");
    avalon::ads::Manager::startService();
    avalon::ads::Manager::enabled = userstate::showAds();
}

void AppDelegate::initLocalization()
{
    auto loca = &avalon::i18n::Localization::getInstance();
    loca->addLanguage("en.ini");
    loca->setCurrentLanguage("en.ini");
    loca->setDefaultLanguage("en.ini");

    auto langId = CCApplication::sharedApplication()->getCurrentLanguage();
    std::string lang;
    if (langId == kLanguageArabic) {
        lang = "ar";
    } else if (langId == kLanguageChinese) {
        lang = "cn";
    } else if (langId == kLanguageEnglish) {
        // already set above
        return;
    } else if (langId == kLanguageFrench) {
        lang = "fr";
    } else if (langId == kLanguageGerman) {
        lang = "de";
    } else if (langId == kLanguageHungarian) {
        lang = "hr";
    } else if (langId == kLanguageItalian) {
        lang = "it";
    } else if (langId == kLanguageJapanese) {
        lang = "ja";
    } else if (langId == kLanguageKorean) {
        lang = "ko";
    } else if (langId == kLanguagePortuguese) {
        lang = "pt";
    } else if (langId == kLanguageRussian) {
        lang = "ru";
    } else if (langId == kLanguageSpanish) {
        lang = "es";
    } else {
        // unknown id .. stick to the default (en)
        return;
    }
    lang += ".ini";

    if (!CCFileUtils::sharedFileUtils()->isFileExist(lang.c_str())) {
        return;
    }

    loca->addLanguage(lang.c_str());
    loca->setCurrentLanguage(lang.c_str());
}