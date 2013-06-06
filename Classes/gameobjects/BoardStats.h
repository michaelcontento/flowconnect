#ifndef __FlowConnect__BoardStats__
#define __FlowConnect__BoardStats__

#include "cocos2d.h"
#include "Board.h"

class GameScene;

class BoardStats : public cocos2d::CCNode
{
public:
    BoardStats();
    virtual ~BoardStats();

    static BoardStats* createWithBoard(Board* board);
    void setBoard(Board* board);
    void updateStats(float dt);

    virtual void onEnter() override;
    virtual void onExit() override;

    void setGameScene(GameScene* gameScene);
    void resetAttackLevelTime();

    static float getAttackLevelTime(const int id);
    static float getAttackPufferTime(const int id);

private:
    bool withBest;
    int movesBest;
    float timeBest;
    float attackTimeLevel;
    float attackTimePuffer;
    bool attackFinished;
    Board* board;
    GameScene* gameScene;
    cocos2d::CCLabelTTF* statsMove;
    cocos2d::CCLabelTTF* statsBest;
    cocos2d::CCLabelTTF* statsProgress;

    bool initWithBoard(Board* board);
};

#endif /* defined(__FlowConnect__BoardStats__) */
