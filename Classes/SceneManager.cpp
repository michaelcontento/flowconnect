#include "SceneManager.h"

using namespace cocos2d;

SceneManager::SceneManager()
: backgroundLayer(NULL)
, previousScene(NULL)
{
    backgroundLayer = CCLayer::create();
    backgroundLayer->retain();
    backgroundLayer->addChild(CCParticleSystemQuad::create("background-fx.plist"));
}

SceneManager::~SceneManager()
{
    CC_SAFE_RELEASE_NULL(backgroundLayer);
    CC_SAFE_RELEASE_NULL(previousScene);
}

CCScene* SceneManager::gotoScene(CCScene* nextScene)
{
    assert(nextScene != NULL && "Scene cannot be null");

    auto currentScene = CCDirector::sharedDirector()->getRunningScene();
    if (currentScene) {
        previousScene = currentScene;
        CC_SAFE_RETAIN(previousScene);

        currentScene->removeChild(backgroundLayer, false);
        nextScene->addChild(backgroundLayer);

        auto transition = CCTransitionFade::create(0.2, nextScene, ccBLACK);
        CCDirector::sharedDirector()->replaceScene(transition);
    } else {
        nextScene->addChild(backgroundLayer);
        CCDirector::sharedDirector()->runWithScene(nextScene);
    }

    return nextScene;
}

void SceneManager::gotoPreviousScene()
{
    if (previousScene) {
        gotoScene(previousScene);
    }
}