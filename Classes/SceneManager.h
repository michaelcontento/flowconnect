#ifndef __FlowConnect__SceneManager__
#define __FlowConnect__SceneManager__

#include "cocos2d.h"
#include "Singleton.h"

class SceneManager : public cocos2d::CCObject, public Singleton<SceneManager>
{
    friend class Singleton<SceneManager>;

public:
    cocos2d::CCScene* gotoScene(cocos2d::CCScene* s);
    cocos2d::CCScene* gotoScene(cocos2d::CCScene* s, const bool storePrevious);
    cocos2d::CCScene* popScene();

private:
    cocos2d::CCArray* sceneStack;
    cocos2d::CCLayer* backgroundLayer;

    SceneManager();
    virtual ~SceneManager();
};

#endif /* defined(__FlowConnect__SceneManager__) */
