#include "AppDelegate.h"

#include "SceneManager.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "Globals.h"
#include "userstate.h"
#include "Localization.h"

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

    initLocalization();

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