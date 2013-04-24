#ifndef __FlowConnect__SettingsScene__
#define __FlowConnect__SettingsScene__

#include "cocos2d.h"
#include "LevelLoader.h"

class SettingsScene : public cocos2d::CCLayer
{
public:
    SettingsScene();
    virtual ~SettingsScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(SettingsScene);
    virtual bool init() override;

    virtual void onEnter() override;

    void btnReset(CCObject* sender);
    void btnHowToPlay();
    void btnMusicToggle();
    void btnSoundToggle();

private:
    const unsigned int MENU_PADDING = 25;
    const LoaderLevel* oldGlobalLevel;
    cocos2d::CCMenu* menu;
};

#endif /* defined(__FlowConnect__SettingsScene__) */
