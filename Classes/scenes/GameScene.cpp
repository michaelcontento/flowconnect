#include "GameScene.h"

#include "../Globals.h"

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
, topMenu(NULL)
, btnMenu(NULL)
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

    addChild(CCParticleSystemQuad::create("background-fx.plist"));

    initBoard();
    initStats();
    initTopMenu();
    initLeftMenu();
    initRightMenu();

    return true;
}

void GameScene::onBtnGoBack()
{
    if (globalLevelLoader.hasPrevious(globalLevel)) {
        globalLevel = globalLevelLoader.getPrevious(globalLevel);

        fadeOutAndRemoveContainer(boardContainer, false);
        initBoard();
        fadeInContainer(boardContainer, false);
    }
}

void GameScene::onBtnReset()
{
    board->reset();
}

void GameScene::onBtnGoNext()
{
    if (globalLevelLoader.hasNext(globalLevel)) {
        globalLevel = globalLevelLoader.getNext(globalLevel);

        fadeOutAndRemoveContainer(boardContainer, true);
        initBoard();
        fadeInContainer(boardContainer, true);
    }
}

void GameScene::onBtnHint()
{
    CCLog("HINT");
}

void GameScene::onBtnHelp()
{
    CCLog("HELP");
}

void GameScene::onBtnMenu()
{
    CCLog("MENU");
}

void GameScene::fadeOutAndRemoveContainer(CCNode* container, const bool toLeft)
{
    auto newPosX = 0 - (BOARD_WIDTH / 2);
    if (!toLeft) {
        newPosX = 768 + (BOARD_WIDTH / 2);
    }

    auto action = CCSequence::create(
        CCEaseIn::create(
            CCScaleTo::create(0.25, container->getScale() * 0.8),
            3
        ),
        CCMoveTo::create(0.8, CCPoint(newPosX, container->getPositionY())),
        CCCallFuncN::create(this, callfuncN_selector(GameScene::removeChild)),
        NULL
    );
    
    container->runAction(action);
}

void GameScene::fadeInContainer(CCNode* container, const bool fromRight)
{
    auto oldPos = container->getPosition();
    auto oldScale = container->getScale();

    container->setScale(oldScale * 0.8);
    container->setPositionX(768 + (BOARD_WIDTH / 2));
    if (!fromRight) {
        container->setPositionX(0 - (BOARD_WIDTH / 2));
    }

    auto action = CCSequence::create(
        CCDelayTime::create(0.25),
        CCMoveTo::create(0.8, oldPos),
        CCEaseIn::create(
            CCScaleTo::create(0.25, oldScale),
            3
        ),
        NULL
    );
    
    container->runAction(action);
}

void GameScene::initBoard()
{
    board = Board::create();
    board->initWithLevel(globalLevel);

    addBoardWithinContainer(board);
}

void GameScene::addBoardWithinContainer(Board* board)
{
    if (stats) {
        stats->setBoard(board);
    }

    boardContainer = CCNode::create();
    boardContainer->addChild(board);
    addChild(boardContainer);

    boardContainer->setContentSize(board->getContentSize());
    boardContainer->setScale(BOARD_WIDTH / board->getContentSize().width);

    boardContainer->setAnchorPoint(CCPoint(0.5, 0.5));
    boardContainer->setPosition(CCPoint(384, 512));
}

void GameScene::initStats()
{
    stats = BoardStats::createWithBoard(board);
    addChild(stats);

    stats->setAnchorPoint(CCPoint(0, 0));
    stats->setPositionX((768 - BOARD_WIDTH) / 2);
    stats->setPositionY(
        boardContainer->getPositionY()
        + boardContainer->getContentSize().height * boardContainer->getScaleY() * 0.5
    );
}

void GameScene::initLeftMenu()
{
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

    leftMenu->setPositionX((768 - BOARD_WIDTH) / 2);
    leftMenu->setPositionY((1024 - BOARD_WIDTH) / 4);
    leftMenu->setAnchorPoint(CCPoint(0, 0.5));

    leftMenu->alignItemsHorizontallyWithPadding(BUTTON_SPACING);
}


void GameScene::initRightMenu()
{
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

    rightMenu->setPositionX((768 - BOARD_WIDTH) / 2 + BOARD_WIDTH);
    rightMenu->setPositionY((1024 - BOARD_WIDTH) / 4);
    rightMenu->setAnchorPoint(CCPoint(1, 0.5));
    
    rightMenu->alignItemsHorizontallyWithPadding(BUTTON_SPACING);
}

void GameScene::initTopMenu()
{
    topMenu = CCMenu::create();
    addChild(topMenu);

    btnMenu = CCSprite::createWithSpriteFrameName("buttons/home.png");
    topMenu->addChild(CCMenuItemSprite::create(
        btnMenu, btnMenu, this, menu_selector(GameScene::onBtnMenu)
    ));

    topMenu->setPositionX((768 - BOARD_WIDTH) / 2);
    topMenu->setPositionY((1024 - BOARD_WIDTH) / 4 * 3 + BOARD_WIDTH);
    topMenu->setAnchorPoint(CCPoint(0, 0.5));

    topMenu->alignItemsHorizontallyWithPadding(BUTTON_SPACING);
}