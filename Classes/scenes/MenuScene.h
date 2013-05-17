#ifndef __FlowConnect__MenuScene__
#define __FlowConnect__MenuScene__

#include "cocos2d.h"

class MenuScene : public cocos2d::CCLayer
{
public:
    MenuScene();
    virtual ~MenuScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(MenuScene);
    virtual bool init() override;

    void btnPlay();
    void btnPLayTimeAttack();
    void btnLeaderboard();
    void btnAchievements();
    void btnShop();
    void btnSettings();
    virtual void keyBackClicked() override;

private:
    static const unsigned int MENU_PADDING = 30;
};

#endif /* defined(__FlowConnect__MenuScene__) */
