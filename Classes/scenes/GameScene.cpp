#include "GameScene.h"

using namespace cocos2d;

#pragma mark Initialization

GameScene::GameScene()
: board(NULL)
, leftMenu(NULL)
, rightMenu(NULL)
, stats(NULL)
, btnGoBack(NULL)
, btnGoNext(NULL)
, btnHelp(NULL)
, btnHint(NULL)
, btnReset(NULL)
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

    // -- BOARD

    board = Board::create();
    //board->initWithLevel(CCSize(4, 4), "ll4lldu1l3udrdurur2u");
    board->initWithLevel(CCSize(5, 5), "3rrrrdudllldrrr4du1rd5dlll2lr6r");

    boardContainer = CCNode::create();
    boardContainer->addChild(board);
    addChild(boardContainer);

    boardContainer->setContentSize(board->getContentSize());
    boardContainer->setScale(BOARD_WIDTH / board->getContentSize().width);
    boardContainer->setAnchorPoint(CCPoint(0, 0));
    boardContainer->setPosition(CCPoint((768 - BOARD_WIDTH) / 2, 150));

    // -- STATS
    
    stats = BoardStats::createWithBoard(board);
    addChild(stats);

    stats->setAnchorPoint(CCPoint(0, 0));
    stats->setPositionX(boardContainer->getPositionX());
    stats->setPositionY(
        boardContainer->getPositionY()
        + boardContainer->getContentSize().height * boardContainer->getScaleY()
    );
    
    // -- LEFT MENU

    leftMenu = CCMenu::create();
    addChild(leftMenu);

    btnGoBack = CCSprite::createWithSpriteFrameName("buttons/go-back.png");
    leftMenu->addChild(CCMenuItemSprite::create(
        btnGoBack, btnGoBack, this, menu_selector(GameScene::onBtnGoBack)
    ));

    btnReset = CCSprite::createWithSpriteFrameName("buttons/reset.png");
    leftMenu->addChild(CCMenuItemSprite::create(
        btnReset, btnReset, this, menu_selector(GameScene::onBtnReset)
    ));

    btnGoNext = CCSprite::createWithSpriteFrameName("buttons/go-next.png");
    leftMenu->addChild(CCMenuItemSprite::create(
        btnGoNext, btnGoNext, this, menu_selector(GameScene::onBtnGoNext)
    ));

    leftMenu->setPosition(boardContainer->getPosition());
    leftMenu->setPositionY(leftMenu->getPositionY() - BOARD_MARGIN);
    leftMenu->setAnchorPoint(CCPoint(0, 1));
    leftMenu->alignItemsHorizontallyWithPadding(BUTTON_SPACING);

    // -- RIGHT MENU

    rightMenu = CCMenu::create();
    addChild(rightMenu);

    btnHint = CCSprite::createWithSpriteFrameName("buttons/hint.png");
    rightMenu->addChild(CCMenuItemSprite::create(
        btnHint, btnHint, this, menu_selector(GameScene::onBtnHint)
    ));

    btnHelp = CCSprite::createWithSpriteFrameName("buttons/help.png");
    rightMenu->addChild(CCMenuItemSprite::create(
        btnHelp, btnHelp, this, menu_selector(GameScene::onBtnHelp)
    ));

    rightMenu->setPosition(boardContainer->getPosition());
    rightMenu->setPositionY(rightMenu->getPositionY() - BOARD_MARGIN);
    rightMenu->setPositionX(
        rightMenu->getPositionX()
        + boardContainer->getContentSize().width * boardContainer->getScaleX()
    );
    rightMenu->setAnchorPoint(CCPoint(1, 1));
    rightMenu->alignItemsHorizontallyWithPadding(BUTTON_SPACING);

    return true;
}

void GameScene::onBtnGoBack()
{
    CCLog("GO BACK");
}

void GameScene::onBtnReset()
{
    board->reset();
}

void GameScene::onBtnGoNext()
{
    CCLog("GO NEXT");
}

void GameScene::onBtnHint()
{
    CCLog("HINT");
}

void GameScene::onBtnHelp()
{
    CCLog("HELP");
}