#ifndef __FlowConnect__SettingsScene__
#define __FlowConnect__SettingsScene__

#include "cocos2d.h"
#include "LevelLoader.h"
#include "AlertView.h"

class SettingsScene : public cocos2d::CCLayer, public AlertViewDelegate
{
public:
    SettingsScene();
    virtual ~SettingsScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(SettingsScene);
    virtual bool init() override;

    virtual void onEnter() override;
    virtual void alertViewClickedButtonAtIndex(int buttonIndex) override;

    void btnReset();
    void btnRemoveAds();
    void btnHowToPlay();
    void btnMoreGames();
    void btnChangeMode();
    void btnMusicToggle();
    void btnSoundToggle();

private:
    const unsigned int MENU_PADDING = 25;
    const LoaderLevel* oldGlobalLevel;
    cocos2d::CCMenu* menu;
    unsigned int mode;
};

#endif /* defined(__FlowConnect__SettingsScene__) */
