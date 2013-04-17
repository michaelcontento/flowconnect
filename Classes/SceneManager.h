#ifndef __FlowConnect__SceneManager__
#define __FlowConnect__SceneManager__

#include "cocos2d.h"
#include "Singleton.h"

class SceneManager : public Singleton<SceneManager>
{
    friend class Singleton<SceneManager>;

public:
    cocos2d::CCScene* gotoScene(cocos2d::CCScene* s);
    void gotoPreviousScene();

private:
    cocos2d::CCScene* previousScene;
    cocos2d::CCLayer* backgroundLayer;

    SceneManager();
    virtual ~SceneManager();
};

#endif /* defined(__FlowConnect__SceneManager__) */
