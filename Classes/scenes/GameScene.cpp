#include "GameScene.h"

#include "../Globals.h"

using namespace cocos2d;

#pragma mark Initialization

GameScene::GameScene()
: board(NULL)
, leftMenu(NULL)
, rightMenu(NULL)
, stats(NULL)
, topMenu(NULL)
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
    initTopMenu();
    initLeftMenu();
    initRightMenu();

    return true;
}

void GameScene::onBtnGoBack()
{
    LoaderLevel* prevLevel = globalLevelLoader.getPrevious(globalLevel);
    if (!prevLevel) {
        LoaderPage* prePage = globalLevelLoader.getPrevious(globalLevel->page);
        if (prePage) {
            prevLevel = prePage->levels.back();
        }
    }

    if (prevLevel) {
        globalLevel = prevLevel;

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
    LoaderLevel* nextLevel = globalLevelLoader.getNext(globalLevel);
    if (!nextLevel) {
        LoaderPage* nextPage = globalLevelLoader.getNext(globalLevel->page);
        if (nextPage) {
            nextLevel = nextPage->levels.front();
        }
    }

    if (nextLevel) {
        globalLevel = nextLevel;

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

    enableMenus(false);
    auto action = CCSequence::create(
        CCDelayTime::create(0.25),
        CCMoveTo::create(0.8, oldPos),
        CCEaseIn::create(
            CCScaleTo::create(0.25, oldScale),
            3
        ),
        CCCallFunc::create(this, callfunc_selector(GameScene::enableMenus)),
        NULL
    );
    
    container->runAction(action);
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