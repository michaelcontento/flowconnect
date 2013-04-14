#include "BoardStats.h"

using namespace cocos2d;

BoardStats::BoardStats()
: statsBest(NULL)
, statsMove(NULL)
, statsProgress(NULL)
, board(NULL)
{
}

BoardStats::~BoardStats()
{
    CC_SAFE_RELEASE_NULL(board);
}

BoardStats* BoardStats::createWithBoard(Board* board)
{
    BoardStats* pRet = new BoardStats();
    if (pRet && pRet->initWithBoard(board)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool BoardStats::initWithBoard(Board* board)
{
    if (!CCNode::init() || !board) {
        return false;
    }

    this->board = board;
    CC_SAFE_RETAIN(board);

    statsMove = CCLabelTTF::create("", "Markler Fett", 24);
    statsMove->setAnchorPoint(CCPoint(0, 0));
    addChild(statsMove);

    statsBest = CCLabelTTF::create("", "Markler Fett", 24);
    statsBest->setAnchorPoint(CCPoint(0.5, 0));
    statsBest->setPositionX(740 / 2);
    addChild(statsBest);

    statsProgress = CCLabelTTF::create("", "Markler Fett", 24);
    statsProgress->setAnchorPoint(CCPoint(1, 0));
    statsProgress->setPositionX(740);
    addChild(statsProgress);

    schedule(schedule_selector(BoardStats::updateStats));

    setContentSize(CCSize(
        board->getContentSize().width,
        statsMove->getContentSize().height
    ));

    return true;
}

void BoardStats::updateStats(float dt)
{
    assert(board && "this should never happen");

    std::stringstream stream;

    stream.str("");
    stream << "moves: " << board->getMoves();
    statsMove->setString(stream.str().c_str());

    stream.str("");
    stream << "time: " << round(board->getDuration() * 100) / 100;
    statsBest->setString(stream.str().c_str());

    stream.str("");
    stream << "solved: " << (board->getProgress() * 100) << "%";
    statsProgress->setString(stream.str().c_str());
}