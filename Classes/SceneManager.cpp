#include "SceneManager.h"

using namespace cocos2d;

SceneManager::SceneManager()
: backgroundLayer(NULL)
, sceneStack(NULL)
{
    sceneStack = CCArray::create();
    sceneStack->retain();

    backgroundLayer = CCLayer::create();
    backgroundLayer->retain();
    backgroundLayer->addChild(CCParticleSystemQuad::create("background-fx.plist"));
}

SceneManager::~SceneManager()
{
    CC_SAFE_RELEASE_NULL(sceneStack);
    CC_SAFE_RELEASE_NULL(backgroundLayer);
}

CCScene* SceneManager::gotoScene(CCScene* nextScene)
{
    return gotoScene(nextScene, true);
}

CCScene* SceneManager::gotoScene(CCScene* nextScene, const bool storePrevious)
{
    assert(nextScene != NULL && "Scene cannot be null");

    auto previousScene = CCDirector::sharedDirector()->getRunningScene();
    if (previousScene) {
        previousScene->removeChild(backgroundLayer, false);

        if (storePrevious) {
            sceneStack->addObject(previousScene);
        }
    }

    nextScene->addChild(backgroundLayer, -INFINITY);

    if (previousScene) {
        CCDirector::sharedDirector()->replaceScene(nextScene);
    } else {
        CCDirector::sharedDirector()->runWithScene(nextScene);
    }

    return nextScene;
}

CCScene* SceneManager::popScene()
{
    if (sceneStack->count() == 0) {
        return NULL;
    }

    auto previousScene = static_cast<CCScene*>(sceneStack->lastObject());
    previousScene->retain();
    sceneStack->removeLastObject();
    
    return gotoScene(previousScene, false);
}