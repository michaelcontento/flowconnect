#ifndef __FlowConnect__SceneManager__
#define __FlowConnect__SceneManager__

#include <avalon/utils/Singleton.h>
#include "cocos2d.h"

class SceneManager
: public cocos2d::CCObject
, public avalon::utils::Singleton<SceneManager>
{
    friend class avalon::utils::Singleton<SceneManager>;

public:
    cocos2d::CCScene* gotoScene(cocos2d::CCScene* s);
    cocos2d::CCScene* gotoScene(cocos2d::CCScene* s, const bool storePrevious);
    cocos2d::CCScene* popScene();
    cocos2d::CCScene* popSceneWithSound();
    cocos2d::CCScene* getLastScene();

private:
    cocos2d::CCScene* currentScene;
    cocos2d::CCArray* sceneStack;
    cocos2d::CCLayer* backgroundLayer;

    SceneManager();
    virtual ~SceneManager();
};

#endif /* defined(__FlowConnect__SceneManager__) */
