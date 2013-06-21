#ifndef __FlowConnect__MenuScene__
#define __FlowConnect__MenuScene__

#include "cocos2d.h"
#include <avalon/utils/platform.h>

class MenuScene : public cocos2d::CCLayer
{
public:
    MenuScene();
    virtual ~MenuScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(MenuScene);
    virtual bool init() override;

    virtual void onEnter() override;

    void btnPlay();
    void btnPLayTimeAttack();
    void btnLeaderboard();
    void btnAchievements();
    void btnShop();
    void btnSettings();
    void btnRemoveAds();
    void btnGoogle(cocos2d::CCObject* object);
    virtual void keyBackClicked() override;

private:
#if AVALON_PLATFORM_IS_ANDROID_GOOGLE
    static const unsigned int MENU_PADDING = 15;
#else
    static const unsigned int MENU_PADDING = 20;
#endif
    static const unsigned int TAG_REMOVEADS = 42;
    cocos2d::CCMenu* menu;
    void showGameCenterDialog();
    void showGoogleLoginDialog();
};

#endif /* defined(__FlowConnect__MenuScene__) */
