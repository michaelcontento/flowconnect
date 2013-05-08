#ifndef __FlowConnect__BoardStats__
#define __FlowConnect__BoardStats__

#include "cocos2d.h"
#include "Board.h"

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

private:
    bool withBest;
    int movesBest;
    float timeBest;
    Board* board;
    cocos2d::CCLabelTTF* statsMove;
    cocos2d::CCLabelTTF* statsBest;
    cocos2d::CCLabelTTF* statsProgress;

    bool initWithBoard(Board* board);
};

#endif /* defined(__FlowConnect__BoardStats__) */
