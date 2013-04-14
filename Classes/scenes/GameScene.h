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

    void onBtnMenu();

private:
    const int BOARD_MARGIN = 25;
    const int BUTTON_SPACING = 30;
    const int BOARD_WIDTH = 696;
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

    cocos2d::CCMenu* topMenu;
    cocos2d::CCSprite* btnMenu;

    void initBoard();
    void initStats();
    void initTopMenu();
    void initLeftMenu();
    void initRightMenu();
};

#endif /* defined(__FlowConnect__GameScene__) */