#include "GameScene.h"

using namespace cocos2d;

#pragma mark Initialization

GameScene::GameScene()
: board(NULL)
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
    board->setPosition(CCPoint(150, 250));
    addChild(board);

    board->initWithLevel(CCSize(4, 4), "ll4lldu1l3udrdurur2u");
    
    return true;
}