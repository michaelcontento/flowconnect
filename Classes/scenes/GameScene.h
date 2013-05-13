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
    virtual bool init() override;

    void onBtnGoBack();
    void onBtnReset();
    void onBtnGoNext();

    void onBtnHint();
    void onBtnHelp();
    void onBtnMenu();

    void enableMenus();
    void enableMenus(const bool flag);

    void onBoardFinished();

private:
    static const int BOARD_MARGIN = 25;
    static const int BUTTON_SPACING = 30;
    static const int BOARD_WIDTH = 696;
    static const int tagAlert = 42;

    cocos2d::CCNode* boardContainer;
    Board* board;
    BoardStats* stats;

    cocos2d::CCLabelTTF* hintLabel;
    cocos2d::CCNode* hintNumber;
    cocos2d::CCLabelTTF* headlineLabel;

    cocos2d::CCMenu* leftMenu;
    cocos2d::CCMenu* rightMenu;

    cocos2d::CCMenuItemSprite* createMenuitem(const char* imagename, cocos2d::CCMenu* menu, cocos2d::SEL_MenuHandler selector);

    void initBoard();
    void initBoardAndFade();
    void addBoardWithinContainer(Board* board);

    void createHintButton(cocos2d::CCMenu* menu);
    void initLabels();
    void initLeftMenu();
    void initRightMenu();
    void updateHintLabel();
};

#endif /* defined(__FlowConnect__GameScene__) */