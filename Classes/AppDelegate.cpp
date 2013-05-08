#include "AppDelegate.h"

#include "SceneManager.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "Globals.h"
#include "userstate.h"
#include "Localization.h"
#include "AdManager.h"
#include "AssetsManager.h"
#include "GameCenter.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace Avalon;

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
    // pDirector->setDisplayStats(true);

    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(pEGLView);
    pEGLView->setDesignResolutionSize(768, 1024, kResolutionExactFit);

    CCSpriteFrameCache::sharedSpriteFrameCache()
        ->addSpriteFramesWithFile("assets.plist");

    initPayment();
    initAds();
    initLocalization();

    auto gc = Avalon::GameCenter();
    gc.login();

    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("background.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("checkpoint-1.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("checkpoint-2.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("checkpoint-3.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("click.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("not-solved.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("solved.mp3");
    if (!userstate::hasMusic()) {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0);
    } else {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background.mp3", true);
    }
    if (!userstate::hasSounds()) {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
    }

    userstate::refreshFreeHints();
    SceneManager::getInstance().gotoScene(MenuScene::scene());

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();

    userstate::refreshFreeHints();

    if (userstate::showAds()) {
        Ads::AdManager::showFullscreenAd();
    }
}

void AppDelegate::initPayment()
{
    Payment::Loader loader("payment.ini");
    Payment::Loader::globalManager = loader.getManager();
}

void AppDelegate::initAds()
{
    auto mgr = new cocos2d::extension::AssetsManager(
        "http://www.coragames.com/apps/dummy/package.zip",
        "http://www.coragames.com/apps/dummy/version"
    );
    mgr->update();
    
    Ads::AdManager::initWithIniFile("ads.ini");
    Ads::AdManager::startService();
    Ads::AdManager::enabled = userstate::showAds();
}

void AppDelegate::initLocalization()
{
    auto loca = &Localization::getInstance();
    
    loca->addLanguage("en.ini");
    loca->setCurrentLanguage("en.ini");
    loca->setDefaultLanguage("en.ini");

    switch (CCApplication::sharedApplication()->getCurrentLanguage()) {
        case kLanguageGerman:
            loca->addLanguage("de.ini");
            loca->setCurrentLanguage("de.ini");
            break;

        case kLanguageFrench:
            loca->addLanguage("fr.ini");
            loca->setCurrentLanguage("fr.ini");
            break;

        default:
            break;
    }
}