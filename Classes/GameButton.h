#ifndef __FlowConnect__GameButton__
#define __FlowConnect__GameButton__

#include "cocos2d.h"
#include "LevelLoader.h"

class GameButton : public cocos2d::CCMenuItem
{
public:
    GameButton();
    virtual ~GameButton();
    static GameButton* createWithLevel(LoaderLevel* level);
    bool initWithLevel(LoaderLevel* level);

    void onClick();
    void setBorderColor(const cocos2d::ccColor3B color);

private:
    LoaderLevel* level;
    cocos2d::CCSprite* border;

    void addBackground();
    void addLabel();
};

#endif /* defined(__FlowConnect__GameButton__) */
