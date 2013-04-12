#include "GameScene.h"

using namespace cocos2d;

#pragma mark Initialization

GameScene::GameScene()
: board(NULL)
, stats(NULL)
{
}

GameScene::~GameScene()
{
}

CCScene* GameScene::scene()
{
    CCScene *scene = CCScene::create();
    scene->addChild(GameScene::create());
    return scene;
}

bool GameScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    CCSpriteFrameCache::sharedSpriteFrameCache()
        ->addSpriteFramesWithFile("assets.plist");

    board = Board::create();
    //board->initWithLevel(CCSize(4, 4), "ll4lldu1l3udrdurur2u");
    board->initWithLevel(CCSize(5, 5), "3rrrrdudllldrrr4du1rd5dlll2lr6r");

    CCNode* boardContainer = CCNode::create();
    addChild(boardContainer);

    boardContainer->addChild(board);
    boardContainer->setContentSize(board->getContentSize());
    boardContainer->setAnchorPoint(CCPoint(0.5, 0.5));

    boardContainer->setPosition(CCPoint(768 / 2, 1024 / 2));
    boardContainer->setScale(700 / board->getContentSize().width);

    stats = BoardStats::createWithBoard(board);
    stats->setAnchorPoint(CCPoint(0, 0));
    stats->setPosition(CCPoint((768 - 700) / 2, 1024 - 162));
    addChild(stats);

    return true;
}