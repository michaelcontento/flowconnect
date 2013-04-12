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
    virtual bool initWithBoard(Board* board);

    void updateStats(float dt);

private:
    Board* board;
    cocos2d::CCLabelTTF* statsMove;
    cocos2d::CCLabelTTF* statsBest;
    cocos2d::CCLabelTTF* statsProgress;
};

#endif /* defined(__FlowConnect__BoardStats__) */
