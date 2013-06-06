#include "BoardStats.h"

#include <avalon/i18n/LanguageKey.h>
#include <avalon/i18n/Localization.h>
#include "GameScene.h"
#include "Globals.h"
#include "userstate.h"

using namespace cocos2d;
using avalon::i18n::_;

BoardStats::BoardStats()
: statsBest(NULL)
, statsMove(NULL)
, statsProgress(NULL)
, board(NULL)
, withBest(false)
, movesBest(0)
, timeBest(0)
, gameScene(NULL)
, attackTimeLevel(0)
, attackTimePuffer(0)
, attackFinished(false)
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

    movesBest = userstate::getLevelMoves(board->getLevel());
    timeBest = userstate::getLevelDuration(board->getLevel());
    withBest = (movesBest > 0);

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
    attackTimeLevel -= dt;
    if (attackTimeLevel < 0) {
        attackTimePuffer = MAX(0, attackTimePuffer + attackTimeLevel);
        attackTimeLevel = 0;
    }

    if (GameScene::mode == GameScene::MODE_TIMEATTACK) {
        statsMove->setString(
            _("game.stats", "attack.puffer")
            .assign("time", MAX(attackTimePuffer, 0))
            .get().c_str()
        );
    } else {
        statsMove->setString(
            _("game.stats", withBest ? "moves.best" : "moves")
            .assign("moves.best", movesBest)
            .assign("moves", board->getMoves())
            .get().c_str()
        );
    }

    if (GameScene::mode == GameScene::MODE_TIMEATTACK) {
        statsBest->setString(
            _("game.stats", "attack.level")
            .assign("time", attackTimeLevel)
            .get().c_str()
        );
    } else {
        statsBest->setString(
            _("game.stats", withBest ? "time.best" : "time")
            .assign("time.best", timeBest, "%.2f")
            .assign("time", board->getDuration(), "%.2f")
            .get().c_str()
        );
    }

    statsProgress->setString(
        _("game.stats", "solved")
        .assign("percent", (int)(board->getProgress() * 100))
        .get().c_str()
    );

    if (!attackFinished && GameScene::mode == GameScene::MODE_TIMEATTACK && (attackTimePuffer + attackTimeLevel) <= 0) {
        gameScene->onTimeAttackTimeout();
        attackFinished = true;
    }
}

void BoardStats::setGameScene(GameScene* gameScene)
{
    this->gameScene = gameScene;
    attackFinished = false;
    attackTimeLevel = getAttackLevelTime(GameScene::timeAttackId);
    attackTimePuffer = getAttackPufferTime(GameScene::timeAttackId);
}

void BoardStats::resetAttackLevelTime()
{
    attackFinished = false;
    attackTimeLevel = getAttackLevelTime(GameScene::timeAttackId);
}

float BoardStats::getAttackLevelTime(const int id)
{
    // 5x5 and 4x4+5x5
    if (id == 1 || id == 3) {
        return 3;
    }
    
    // 6x6 and 5x5+6x6
    if (id == 2 || id == 4) {
        return 5;
    }

    // 4x4 and default
    return 2;
}

float BoardStats::getAttackPufferTime(const int id)
{
    // 5x5 and 4x4+5x5
    if (id == 1 || id == 3) {
        return 20;
    }

    // 6x6 and 5x5+6x6
    if (id == 2 || id == 4) {
        return 25;
    }

    // 4x4 and default
    return 15;
}