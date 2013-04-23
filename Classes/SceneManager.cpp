#include "SceneManager.h"

#include "ButtonFactory.h"

using namespace cocos2d;

SceneManager::SceneManager()
: backgroundLayer(NULL)
, sceneStack(NULL)
{
    sceneStack = CCArray::create();
    sceneStack->retain();

    backgroundLayer = CCLayer::create();
    backgroundLayer->retain();

    auto bg = CCParticleSystemQuad::create("background-fx.plist");
    backgroundLayer->addChild(bg);

    // fast forward some frames
    for (int i = 0; i < 25; ++i) {
        bg->update(0.16);
    }
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

    ButtonFactory::resetColorCounter();
    if (previousScene) {
        CCDirector::sharedDirector()->replaceScene(nextScene);
    } else {
        CCDirector::sharedDirector()->runWithScene(nextScene);
    }

    return nextScene;
}

CCScene* SceneManager::popScene()
{
    auto previousScene = getLastScene();
    previousScene->retain();
    sceneStack->removeLastObject();
    
    return gotoScene(previousScene, false);
}

CCScene* SceneManager::getLastScene()
{
    if (sceneStack->count() == 0) {
        return NULL;
    }

    return static_cast<CCScene*>(sceneStack->lastObject());
}