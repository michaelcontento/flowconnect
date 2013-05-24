#ifndef __FlowConnect__GameButton__
#define __FlowConnect__GameButton__

#include "cocos2d.h"
#include "../LevelLoader.h"
#include "../userstate.h"
#include "../buttons/PageLockButton.h"

class GameButton : public cocos2d::CCMenuItem
{
public:
    GameButton();
    virtual ~GameButton();
    static GameButton* createWithLevel(const LoaderLevel* level, PageLockButton* pagelock);
    bool initWithLevel(const LoaderLevel* level, PageLockButton* pagelock);

    void onClick();
    void setBorderColor(const cocos2d::ccColor3B color);
    void updateStateIndicator();

private:
    const LoaderLevel* level;
    PageLockButton* pagelock;
    userstate::Mode::Enum lastState;
    cocos2d::CCSprite* star;
    cocos2d::CCSprite* border;
    cocos2d::CCLabelBMFont* label;

    void addBackground();
    void addLabel();
};

#endif /* defined(__FlowConnect__GameButton__) */
