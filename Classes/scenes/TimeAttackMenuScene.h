#ifndef __FlowConnect__TimeAttackScene__
#define __FlowConnect__TimeAttackScene__

#include "cocos2d.h"

class LoaderLevel;

class TimeAttackMenuScene : public cocos2d::CCLayer
{
public:
    TimeAttackMenuScene();
    virtual ~TimeAttackMenuScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(TimeAttackMenuScene);
    virtual bool init() override;

    virtual void onEnter() override;
    virtual void keyBackClicked() override;

    void btnGame(void* sender);

private:
    static const unsigned int MENU_PADDING = 25;
    cocos2d::CCMenu* menu;

    void createMenu();
    void createButton(const int id);
    void setGlobalLevelAndAttackLevelQueue(const int id);
    std::vector<LoaderLevel*> getLevelsForCategory(const int id);
    std::vector<LoaderLevel*> getMergedLevels(const int a, const int b);
};

#endif /* defined(__FlowConnect__TimeAttackScene__) */
