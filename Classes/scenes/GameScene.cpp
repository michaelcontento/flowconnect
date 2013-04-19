#include "GameScene.h"

#include "SceneManager.h"
#include "../Colors.h"

using namespace cocos2d;

extern LoaderLevel* globalLevel;

#pragma mark Initialization

GameScene::GameScene()
: board(NULL)
, leftMenu(NULL)
, rightMenu(NULL)
, stats(NULL)
, topMenu(NULL)
, sizeLabel(NULL)
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

    initTopMenu();
    initLeftMenu();
    initRightMenu();
    initLabels();
    
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
    CCLog("HINT");
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
    auto menus = CCArray::create();
    menus->addObject(topMenu);
    menus->addObject(leftMenu);
    menus->addObject(rightMenu);

    CCObject* menuIt;
    CCARRAY_FOREACH(menus, menuIt) {
        auto menu = static_cast<CCMenu*>(menuIt);
        
        CCObject* itemIt;
        CCARRAY_FOREACH(menu->getChildren(), itemIt) {
            auto item = static_cast<CCMenuItemSprite*>(itemIt);
            item->setEnabled(flag);
        }
    }
}

void GameScene::initBoard()
{
    board = Board::create();
    board->initWithLevel(globalLevel);
    addBoardWithinContainer(board);

    headlineLabel->setString(globalLevel->page->name);

    char buf[50] = {0};
    snprintf(
        buf, 50, "%.0fx%.0f #%d",
        board->getSize().width,
        board->getSize().height,
        globalLevel->localid
    );
    sizeLabel->setString(buf);
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

    auto disabled = CCSprite::createWithSpriteFrameName(imagename);
    disabled->setOpacity(DISABLED_OPACITY);
    
    menu->addChild(CCMenuItemSprite::create(normal, normal, disabled, this, selector));
}

void GameScene::initLabels()
{
    auto container = CCNode::create();
    container->setPositionX(768 / 2);
    container->setPositionY(topMenu->getPositionY());
    container->setAnchorPoint(CCPoint(0.5, 0.5));
    addChild(container);

    headlineLabel = CCLabelTTF::create("", "Markler Fett", 58);
    headlineLabel->setAnchorPoint(CCPoint(0.5, 0));
    headlineLabel->setPositionY(-5);
    container->addChild(headlineLabel);

    sizeLabel = CCLabelTTF::create("", "Markler Fett", 28);
    sizeLabel->setOpacity(DISABLED_OPACITY);
    sizeLabel->setAnchorPoint(CCPoint(0.5, 1));
    sizeLabel->setPositionY(5);
    container->addChild(sizeLabel);
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

void GameScene::initTopMenu()
{
    topMenu = CCMenu::create();
    addChild(topMenu);

    createMenuitem("buttons/home.png", topMenu, menu_selector(GameScene::onBtnMenu));

    topMenu->setPositionX((768 - BOARD_WIDTH) / 2);
    topMenu->setPositionY((1024 - BOARD_WIDTH) / 4 * 3 + BOARD_WIDTH);
    topMenu->setAnchorPoint(CCPoint(0, 0.5));

    topMenu->alignItemsHorizontallyWithPadding(BUTTON_SPACING);
}