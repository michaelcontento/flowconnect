#include "SceneManager.h"

#include "../buttons/ButtonFactory.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

SceneManager::SceneManager()
: backgroundLayer(NULL)
, sceneStack(NULL)
, currentScene(NULL)
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

    // this happens if the last scene-change-request hasn't been switched
    // inside cocos2d yet. just search for m_pRunningScene in CCDirector and
    // have a look at the actual point of change. you'll notice that
    // getRunningScene() could point to the "previous" scene and that would
    // cause the "move background node to the next scene" logic below to
    // crash, as it would be added to the same node twice. for this app it's
    // safe to just ignore the change request :)
    if (previousScene != currentScene) {
        return currentScene;
    }
    
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
    currentScene = nextScene;

    return nextScene;
}

CCScene* SceneManager::popScene()
{
    auto previousScene = getLastScene();
    previousScene->retain();
    sceneStack->removeLastObject();
    
    return gotoScene(previousScene, false);
}

CCScene* SceneManager::popSceneWithSound()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");
    return popScene();
}

CCScene* SceneManager::getLastScene()
{
    if (sceneStack->count() == 0) {
        return NULL;
    }

    return static_cast<CCScene*>(sceneStack->lastObject());
}