#include "BoardStats.h"

#include "Globals.h"
#include "Localization.h"
#include "LanguageKey.h"

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

    statsMove = CCLabelTTF::create("", SMALL_FONT_NAME, 24);
    statsMove->setAnchorPoint(CCPoint(0, 0));
    addChild(statsMove);

    statsBest = CCLabelTTF::create("", SMALL_FONT_NAME, 24);
    statsBest->setAnchorPoint(CCPoint(0.5, 0));
    statsBest->setPositionX(696 / 2);
    addChild(statsBest);

    statsProgress = CCLabelTTF::create("", SMALL_FONT_NAME, 24);
    statsProgress->setAnchorPoint(CCPoint(1, 0));
    statsProgress->setPositionX(696);
    addChild(statsProgress);

    setBoard(board);

    return true;
}

void BoardStats::onEnter()
{
    CCNode::onEnter();

    schedule(schedule_selector(BoardStats::updateStats), 0.1);
}

void BoardStats::onExit()
{
    unschedule(schedule_selector(BoardStats::updateStats));

    CCNode::onExit();
}

void BoardStats::setBoard(Board* board)
{
    CC_SAFE_RELEASE_NULL(this->board);

    this->board = board;
    this->board->retain();

    setContentSize(CCSize(
        statsProgress->getPositionX(),
        statsMove->getContentSize().height
    ));
}

void BoardStats::updateStats(float dt)
{
    assert(board && "this should never happen");
    static char tmp[25] = {0};
    
    snprintf(
        tmp, sizeof(tmp),
        _("game.stats", "moves")->getCString(),
        board->getMoves()
    );
    statsMove->setString(tmp);

    snprintf(
        tmp, sizeof(tmp),
        _("game.stats", "time")->getCString(),
        board->getDuration()
    );
    statsBest->setString(tmp);

    snprintf(
        tmp, sizeof(tmp),
        _("game.stats", "solved")->getCString(),
        board->getProgress() * 100
    );
    statsProgress->setString(tmp);
}