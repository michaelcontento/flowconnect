#include "GameScene.h"

#include <avalon/ads/Manager.h>
#include <avalon/i18n/LanguageKey.h>
#include <avalon/i18n/Localization.h>
#include "SceneManager.h"
#include "../buttons/ButtonFactory.h"
#include "../Colors.h"
#include "Globals.h"
#include "userstate.h"
#include "LevelMenuScene.h"
#include "Alert.h"
#include "SimpleAudioEngine.h"

int GameScene::mode = GameScene::MODE_NORMAL;
int GameScene::timeAttackId = 0;

using namespace cocos2d;
using avalon::i18n::_;

void GameScene::onEnter()
{
    CCLayer::onEnter();
    avalon::ads::Manager::hide();
}

void GameScene::createHintButton(cocos2d::CCMenu* menu)
{
    auto bg = CCSprite::createWithSpriteFrameName("buttons/hint-number.png");
    bg->setAnchorPoint(CCPointZero);

    hintLabel = CCLabelTTF::create("0", DEFAULT_FONT_NAME, 24);
    hintLabel->setPosition(ccpMult(ccpFromSize(bg->getContentSize()), 0.5));
    hintLabel->setAnchorPoint(CCPoint(0.5, 0.5));
    hintLabel->setPositionY(hintLabel->getPositionY() + 1);

    auto hint = createMenuitem("buttons/hint.png", menu, menu_selector(GameScene::onBtnHint));
    hintNumber = CCNode::create();
    hintNumber->addChild(bg);
    hintNumber->addChild(hintLabel);
    hintNumber->setContentSize(bg->getContentSize());
    hintNumber->setAnchorPoint(CCPoint(0.5, 0.5));
    hintNumber->setPosition(
        hint->getContentSize().width - 5,
        hint->getContentSize().height - 5
    );
    hint->addChild(hintNumber);
    updateHintLabel();
}

#pragma mark Initialization

GameScene::GameScene()
: board(NULL)
, leftMenu(NULL)
, rightMenu(NULL)
, stats(NULL)
, headlineLabel(NULL)
, hintLabel(NULL)
, hintNumber(NULL)
, attackLevel(1)
{
}

GameScene::~GameScene()
{
    CC_SAFE_RELEASE_NULL(stats);
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

    setKeypadEnabled(true);
    return true;
}

void GameScene::keyBackClicked()
{
    SceneManager::getInstance().popSceneWithSound();
}

void GameScene::onBtnGoBack()
{
    LoaderLevel* prevLevel = globalLevel->prev;
    if (!prevLevel) {
        LoaderPage* prePage = globalLevel->page->prev;
        if (prePage) {
            if (!userstate::isPageFree(prePage)) {
                LevelMenuScene::scrollTo = prePage;
                SceneManager::getInstance().popScene();
            } else {
                prevLevel = prePage->levels.back();
            }
        }
    }

    if (prevLevel) {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");
        globalLevel = prevLevel;
        initBoardAndFade();
    }
}

void GameScene::onBtnReset()
{
    if (board->getMoves() == 0) {
        return;
    }

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");

    auto alert = getChildByTag(tagAlert);
    if (alert) {
        if (userstate::showAds()) {
            avalon::ads::Manager::hide();
            avalon::ads::Manager::showFullscreenAd();
        }
        removeChild(alert);
    }
    board->reset();
}

void GameScene::onBtnGoNext()
{
    auto alert = getChildByTag(tagAlert);
    if (alert) {
        if (userstate::showAds()) {
            avalon::ads::Manager::hide();
            avalon::ads::Manager::showFullscreenAd();
        }
        removeChild(alert);
    }

    LoaderLevel* nextLevel = globalLevel->next;
    if (!nextLevel) {
        LoaderPage* nextPage = globalLevel->page->next;
        if (nextPage) {
            if (!userstate::isPageFree(nextPage)) {
                LevelMenuScene::scrollTo = nextPage;
                SceneManager::getInstance().popScene();
            } else {
                nextLevel = nextPage->levels.front();
            }
        }
    }

    if (nextLevel) {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");
        globalLevel = nextLevel;
        initBoardAndFade();
    }
}

void GameScene::onBtnHint()
{
    if (!board->hasPendingCheckpoint()) {
        return;
    }

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");

    auto freeHints = userstate::getFreeHints();
    auto showWarning = userstate::showHintWarning();

    if (freeHints == 0 && showWarning) {
        CCMessageBox(
            _("dialog.hintwarning", "body").assign("amount", PRICE_HINT).get().c_str(),
            _("dialog.hintwarning", "headline").get().c_str()
        );
        userstate::setHintWarning(false);
        return;
    }

    if (freeHints == 0 && !userstate::addStarsToUser(PRICE_HINT * -1)) {
        return;
    }

    if (board->finishTillNextCheckpoint() && freeHints > 0) {
        userstate::addFreeHint(-1);
        updateHintLabel();
    }
}

void GameScene::onBtnHelp()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");
    CCLog("HELP");
}

void GameScene::onBtnMenu()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");
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
    board->initWithLevel(globalLevel, this);
    addBoardWithinContainer(board);

    if (headlineLabel) {
        removeChild(headlineLabel);
    }

    if (mode == MODE_NORMAL) {
        headlineLabel = ButtonFactory::createHeadline(globalLevel->getLocalizedName());
    } else {
        headlineLabel = ButtonFactory::createHeadline(
            _("game", "headline.attack")
            .assign("level", attackLevel)
            .get().c_str()
        );
    }
    addChild(headlineLabel);
}

void GameScene::addBoardWithinContainer(Board* board)
{
    auto boardContainerInner = CCNode::create();
    boardContainerInner->setContentSize(board->getContentSize());
    boardContainerInner->setScale(BOARD_WIDTH / board->getContentSize().width);
    boardContainerInner->setAnchorPoint(CCPoint(0.5, 0.5));
    boardContainerInner->addChild(board);
    board->setTouchIndicatorScale(boardContainerInner->getScale());

    if (!stats) {
        stats = BoardStats::createWithBoard(board);
        stats->retain();
        stats->setGameScene(this);
    } else {
        stats->setBoard(board);
    }
    stats->setAnchorPoint(CCPoint(0.5, 0));
    stats->setPositionY(BOARD_WIDTH / 2);

    boardContainer = CCNode::create();
    boardContainer->setAnchorPoint(CCPoint(0.5, 0.5));
    boardContainer->setPosition(CCPoint(384, 512));
    boardContainer->addChild(boardContainerInner);
    boardContainer->addChild(stats);
    addChild(boardContainer);
}

CCMenuItemSprite* GameScene::createMenuitem(const char* imagename, CCMenu* menu, SEL_MenuHandler selector)
{
    auto normal = CCSprite::createWithSpriteFrameName(imagename);
    auto child = CCMenuItemSprite::create(normal, normal, normal, this, selector);

    menu->addChild(child);
    return child;
}

void GameScene::initRightMenu()
{
    rightMenu = CCMenu::create();
    addChild(rightMenu);

    if (mode == MODE_NORMAL) {
        createMenuitem("buttons/go-back.png", rightMenu, menu_selector(GameScene::onBtnGoBack));
    }
    createMenuitem("buttons/reset.png", rightMenu, menu_selector(GameScene::onBtnReset));
    createMenuitem("buttons/go-next.png", rightMenu, menu_selector(GameScene::onBtnGoNext));

    rightMenu->setPositionX((768 - BOARD_WIDTH) / 2 + BOARD_WIDTH);
    rightMenu->setPositionY((1024 - BOARD_WIDTH) / 4);
    rightMenu->setAnchorPoint(CCPoint(1, 0.5));

    rightMenu->alignItemsHorizontallyWithPadding(BUTTON_SPACING);
}

void GameScene::updateHintLabel()
{
    auto hints = userstate::getFreeHints();

    if (hints == 0) {
        hintNumber->setVisible(false);
    } else {
        hintNumber->setVisible(true);

        char buf[10] = {0};
        snprintf(buf, sizeof(buf), "%d", hints);
        hintLabel->setString(buf);
    }
}

void GameScene::initLeftMenu()
{
    leftMenu = CCMenu::create();
    addChild(leftMenu);

    //createMenuitem("buttons/help.png", leftMenu, menu_selector(GameScene::onBtnHelp));

    createHintButton(leftMenu);
    leftMenu->setPositionX((768 - BOARD_WIDTH) / 2);
    leftMenu->setPositionY((1024 - BOARD_WIDTH) / 4);
    leftMenu->setAnchorPoint(CCPoint(0, 0.5));

    leftMenu->alignItemsHorizontallyWithPadding(BUTTON_SPACING);
}

void GameScene::onTimeAttackTimeout()
{
    avalon::ads::Manager::showBannerIgnoreTime();
    board->forceCancelTouch();
    
    auto alert = Alert::create();
    alert->setTag(tagAlert);
    addChild(alert);

    userstate::setScoreForTimeAttack(timeAttackId, attackLevel - 1);

    alert->setHeadline(_("alert.attacktimeout", "headline").get().c_str());
    alert->setBody(
        _("alert.attacktimeout", "body")
        .assign("amount", attackLevel)
        .get().c_str()
    );
    alert->addButton(
        _("alert.attacktimeout", "btn.extratime").get().c_str(),
        this, menu_selector(GameScene::onBtnAttackExtraTime)
    );
    alert->addButton(
        _("alert.attacktimeout", "btn.close").get().c_str(),
        this, menu_selector(GameScene::onBtnClose)
    );
}

void GameScene::onBtnAttackExtraTime()
{
    if (userstate::addStarsToUser(-2)) {
        stats->resetAttackLevelTime();
        removeChildByTag(tagAlert);
        avalon::ads::Manager::hide();
    }
}

void GameScene::onBtnClose()
{
    removeChildByTag(tagAlert);
    avalon::ads::Manager::hide();
    avalon::ads::Manager::showFullscreenAd();
    SceneManager::getInstance().popSceneWithSound();
}

void GameScene::onBoardFinished()
{
    if (mode == MODE_TIMEATTACK) {
        attackLevel += 1;
        onBtnGoNext();
        stats->resetAttackLevelTime();
        return;
    }

    avalon::ads::Manager::showBannerIgnoreTime();

    auto lastState = userstate::getModeForLevel(board->getLevel());
    auto moves = board->getMoves();
    auto perfectMoves = (board->getSize().width * board->getSize().height) - 1;
    auto headlineKey = std::string("headline");

    if (moves == perfectMoves) {
        headlineKey += ".perfect";
        userstate::setModeForLevel(board->getLevel(), userstate::Mode::PERFECT);
    } else if (lastState == userstate::Mode::PERFECT) {
        userstate::setModeForLevel(board->getLevel(), userstate::Mode::PERFECT);
    } else {
        userstate::setModeForLevel(board->getLevel(), userstate::Mode::SOLVED);
    }

    auto alert = Alert::create();
    alert->setTag(tagAlert);
    addChild(alert);

    alert->setHeadline(_("alert.gamesolved", headlineKey.c_str()).get().c_str());
    alert->setBody(
        _("alert.gamesolved", (lastState == userstate::Mode::NONE) ? "body.first" : "body")
        .assign("moves.best", userstate::getLevelMoves(board->getLevel()))
        .assign("moves", moves)
        .assign("time.best", userstate::getLevelDuration(board->getLevel()))
        .assign("time", board->getDuration())
        .get().c_str()
    );
    alert->addButton(
        _("alert.gamesolved", "btn.next").get().c_str(),
        this, menu_selector(GameScene::onBtnGoNext)
    );
    alert->addButton(
        _("alert.gamesolved", "btn.again").get().c_str(),
        this, menu_selector(GameScene::onBtnReset)
    );

    userstate::updateLevelDuration(board->getLevel(), board->getDuration());
    userstate::updateLevelMoves(board->getLevel(), board->getMoves());
}
