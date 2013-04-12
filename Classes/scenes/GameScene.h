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

private:
    Board* board;
    BoardStats* stats;
    cocos2d::CCNode* boardContainer;

    void doLayout();
};

#endif /* defined(__FlowConnect__GameScene__) */