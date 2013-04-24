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
    virtual bool init() override;

    virtual void onEnter() override;

    void btnGame(void* sender);

private:
    const unsigned int MENU_PADDING = 25;
    cocos2d::CCMenu* menu;
    int lastStars;

    void fillMenu();
};

#endif /* defined(__FlowConnect__CategoryMenuScene__) */
