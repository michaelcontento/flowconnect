#include "GameScene.h"

#include "SceneManager.h"
#include "ButtonFactory.h"
#include "../Colors.h"

using namespace cocos2d;

extern LoaderLevel* globalLevel;

#pragma mark Initialization

GameScene::GameScene()
: board(NULL)
, leftMenu(NULL)
, rightMenu(NULL)
, stats(NULL)
, headlineLabel(NULL)
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

    addChild(ButtonFactory::createSceneBackButton());
    addChild(ButtonFactory::createStar());
    initLeftMenu();
    initRightMenu();
    
    initBoard();

    return true;
}

void GameScene::onBtnGoBack()
{
    LoaderLevel* prevLevel = globalLevel->prev;
    if (!prevLevel) {
        LoaderPage* prePage = globalLevel->page->prev;
        if (prePage) {
            prevLevel = prePage->levels.back();
        }
    }

    if (prevLevel) {
        globalLevel = prevLevel;
        initBoardAndFade();
    }
}

void GameScene::onBtnReset()
{
    board->reset();
}

void GameScene::onBtnGoNext()
{
    LoaderLevel* nextLevel = globalLevel->next;
    if (!nextLevel) {
        LoaderPage* nextPage = globalLevel->page->next;
        if (nextPage) {
            nextLevel = nextPage->levels.front();
        }
    }

    if (nextLevel) {
        globalLevel = nextLevel;
        initBoardAndFade();
    }
}

void GameScene::onBtnHint()
{
    auto freeHints = userstate::getFreeHints();
    auto stars = userstate::getStarsForUser();
    auto showWarning = userstate::showHintWarning();

    if (freeHints == 0 && showWarning) {
        CCLog("WARNING!");
        userstate::setHintWarning(false);
        return;
    }

    if (freeHints == 0 && stars == 0) {
        CCLog("NOT ENOUGH STARS");
        return;
    }

    if (board->finishTillNextCheckpoint()) {
        if (freeHints > 0) {
            userstate::addFreeHint(-1);
        } else {
            userstate::addStarsToUser(-1);
        }
    }
}

void GameScene::onBtnHelp()
{
    CCLog("HELP");
}

void GameScene::onBtnMenu()
{
    SceneManager::getInstance().popScene();
}

void GameScene::initBoardAndFade()
{
    removeChild(boardContainer);
    initBoard();

    auto oldScale = boardContainer->getScale();
    boardContainer->setScale(boardContainer->getScale() * 0.5);

    auto action = CCSequence::create(
        CCEaseOut::create(CCScaleTo::create(0.2, oldScale), 1),
        CCCallFunc::create(this, callfunc_selector(GameScene::enableMenus)),
        NULL
    );

    enableMenus(false);
    boardContainer->runAction(action);
}

void GameScene::enableMenus()
{
    enableMenus(true);
}

void GameScene::enableMenus(const bool flag)
{
    leftMenu->setEnabled(flag);
    rightMenu->setEnabled(flag);
}

void GameScene::initBoard()
{
    board = Board::create();
    board->initWithLevel(globalLevel);
    addBoardWithinContainer(board);

    if (headlineLabel) {
        removeChild(headlineLabel);
    }
    char buf[50] = {0};
    snprintf(
        buf, 50, "%s %s #%d",
        globalLevel->page->category->name,
        globalLevel->page->name,
        globalLevel->localid
    );
    headlineLabel = ButtonFactory::createHeadline(buf);
    addChild(headlineLabel);
}

void GameScene::addBoardWithinContainer(Board* board)
{
    auto boardContainerInner = CCNode::create();
    boardContainerInner->setContentSize(board->getContentSize());
    boardContainerInner->setScale(BOARD_WIDTH / board->getContentSize().width);
    boardContainerInner->setAnchorPoint(CCPoint(0.5, 0.5));
    boardContainerInner->addChild(board);

    stats = BoardStats::createWithBoard(board);
    stats->setAnchorPoint(CCPoint(0.5, 0));
    stats->setPositionY(BOARD_WIDTH / 2);

    boardContainer = CCNode::create();
    boardContainer->setAnchorPoint(CCPoint(0.5, 0.5));
    boardContainer->setPosition(CCPoint(384, 512));
    boardContainer->addChild(boardContainerInner);
    boardContainer->addChild(stats);
    addChild(boardContainer);
}

void GameScene::createMenuitem(const char* imagename, CCMenu* menu, SEL_MenuHandler selector)
{
    auto normal = CCSprite::createWithSpriteFrameName(imagename);
    menu->addChild(CCMenuItemSprite::create(normal, normal, normal, this, selector));
}

void GameScene::initLeftMenu()
{
    leftMenu = CCMenu::create();
    addChild(leftMenu);

    createMenuitem("buttons/go-back.png", leftMenu, menu_selector(GameScene::onBtnGoBack));
    createMenuitem("buttons/reset.png", leftMenu, menu_selector(GameScene::onBtnReset));
    createMenuitem("buttons/go-next.png", leftMenu, menu_selector(GameScene::onBtnGoNext));

    leftMenu->setPositionX((768 - BOARD_WIDTH) / 2);
    leftMenu->setPositionY((1024 - BOARD_WIDTH) / 4);
    leftMenu->setAnchorPoint(CCPoint(0, 0.5));

    leftMenu->alignItemsHorizontallyWithPadding(BUTTON_SPACING);
}


void GameScene::initRightMenu()
{
    rightMenu = CCMenu::create();
    addChild(rightMenu);

    createMenuitem("buttons/hint.png", rightMenu, menu_selector(GameScene::onBtnHint));
    createMenuitem("buttons/help.png", rightMenu, menu_selector(GameScene::onBtnHelp));

    rightMenu->setPositionX((768 - BOARD_WIDTH) / 2 + BOARD_WIDTH);
    rightMenu->setPositionY((1024 - BOARD_WIDTH) / 4);
    rightMenu->setAnchorPoint(CCPoint(1, 0.5));
    
    rightMenu->alignItemsHorizontallyWithPadding(BUTTON_SPACING);
}