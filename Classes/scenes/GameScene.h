#ifndef __FlowConnect__GameScene__
#define __FlowConnect__GameScene__

#include "cocos2d.h"
#include "Board.h"
#include "BoardStats.h"

class GameScene : public cocos2d::CCLayer
{
public:
    GameScene();
    virtual ~GameScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(GameScene);
    virtual bool init();

    void onBtnGoBack();
    void onBtnReset();
    void onBtnGoNext();

    void onBtnHint();
    void onBtnHelp();

private:
    const int BOARD_MARGIN = 20;
    const int BUTTON_SPACING = 30;
    BoardStats* stats;

    cocos2d::CCNode* boardContainer;
    Board* board;

    cocos2d::CCMenu* leftMenu;
    cocos2d::CCSprite* btnGoBack;
    cocos2d::CCSprite* btnReset;
    cocos2d::CCSprite* btnGoNext;

    cocos2d::CCMenu* rightMenu;
    cocos2d::CCSprite* btnHint;
    cocos2d::CCSprite* btnHelp;
};

#endif /* defined(__FlowConnect__GameScene__) */