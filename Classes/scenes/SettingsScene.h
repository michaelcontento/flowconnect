#ifndef __FlowConnect__SettingsScene__
#define __FlowConnect__SettingsScene__

#include <avalon/ui/AlertDelegate.h>
#include "cocos2d.h"
#include "LevelLoader.h"

class SettingsScene : public cocos2d::CCLayer, public avalon::ui::AlertDelegate
{
public:
    SettingsScene();
    virtual ~SettingsScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(SettingsScene);
    virtual bool init() override;

    virtual void onEnter() override;
    virtual void onAlertButtonClick(const unsigned int index, const std::string title) override;

    void btnReset();
    void btnRemoveAds();
    void btnHowToPlay();
    void btnMoreGames();
    void btnChangeMode(cocos2d::CCObject* sender);
    void btnMusicToggle(cocos2d::CCObject* sender);
    void btnSoundToggle(cocos2d::CCObject* sender);
    virtual void keyBackClicked() override;

private:
    static const unsigned int MENU_PADDING = 20;
    const LoaderLevel* oldGlobalLevel;
    cocos2d::CCMenu* menu;
    unsigned int mode;
};

#endif /* defined(__FlowConnect__SettingsScene__) */
