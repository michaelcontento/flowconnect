#ifndef __FlowConnect__GameScene__
#define __FlowConnect__GameScene__

#include "cocos2d.h"
#include "Board.h"
#include "BoardStats.h"
#include "../Colors.h"

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

    void enableMenus();
    void enableMenus(const bool flag);

private:
    const int BOARD_MARGIN = 25;
    const int BUTTON_SPACING = 30;
    const int BOARD_WIDTH = 696;

    cocos2d::CCNode* boardContainer;
    Board* board;
    BoardStats* stats;

    cocos2d::CCMenu* leftMenu;
    cocos2d::CCMenu* rightMenu;
    cocos2d::CCMenu* topMenu;

    void createMenuitem(const char* imagename, cocos2d::CCMenu* menu, cocos2d::SEL_MenuHandler selector);

    void initBoard();
    void addBoardWithinContainer(Board* board);
    void fadeOutAndRemoveContainer(cocos2d::CCNode* container, const bool toLeft);
    void fadeInContainer(cocos2d::CCNode* container, const bool fromRight);
    
    void initTopMenu();
    void initLeftMenu();
    void initRightMenu();
};

#endif /* defined(__FlowConnect__GameScene__) */