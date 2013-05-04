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
#include "Loader.h"
#include "Manager.h"
#include "Product.h"
#include "ProductConsumable.h"

using namespace cocos2d;
using namespace CocosDenshion;

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
    CCLog("COUNT %d", Avalon::Payment::Loader::globalManager.use_count());
    initAds();
    initLocalization();

    auto gc = Avalon::GameCenter();
    gc.login();

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
}

void AppDelegate::initPayment()
{
    Avalon::Payment::Loader loader("payment.ini");
    Avalon::Payment::Loader::globalManager = loader.getManager();
    CCLog("COUNT %d", Avalon::Payment::Loader::globalManager.use_count());
}

void AppDelegate::initAds()
{
    if (!userstate::showAds()) {
        Ads::AdManager::disableAds();
        return;
    }

    auto mgr = new cocos2d::extension::AssetsManager(
        "http://www.coragames.com/apps/dummy/package.zip",
        "http://www.coragames.com/apps/dummy/version"
    );
    mgr->update();
    
    Ads::AdManager::initWithIniFile("ads.ini");
    Ads::AdManager::startService();
    Ads::AdManager::enableAds();
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