#ifndef __FlowConnect__CategoryMenuScene__
#define __FlowConnect__CategoryMenuScene__

#include "cocos2d.h"

class CategoryMenuScene : public cocos2d::CCLayer
{
public:
    CategoryMenuScene();
    virtual ~CategoryMenuScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(CategoryMenuScene);
    virtual bool init();

    void btnGame(void* sender);
};

#endif /* defined(__FlowConnect__CategoryMenuScene__) */
