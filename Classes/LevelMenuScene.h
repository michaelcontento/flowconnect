#ifndef __FlowConnect__LevelMenuScene__
#define __FlowConnect__LevelMenuScene__

#include "cocos2d.h"

class LevelMenuScene : public cocos2d::CCLayer
{
public:
    LevelMenuScene();
    virtual ~LevelMenuScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(LevelMenuScene);
    virtual bool init();

    void btnGame(void* sender) const;
    cocos2d::CCNode* createMenuContainer();
    void alignMenu(cocos2d::CCMenu* menu) const;

private:
    const unsigned int ITEMS_PER_ROW = 6;
};

#endif /* defined(__FlowConnect__LevelMenuScene__) */
