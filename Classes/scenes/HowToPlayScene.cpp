#include "HowToPlayScene.h"

#include <avalon/i18n/LanguageKey.h>
#include <avalon/i18n/Localization.h>
#include "Board.h"
#include "Globals.h"
#include "GameScene.h"
#include "ButtonFactory.h"
#include "SceneManager.h"
#include "SimpleAudioEngine.h"
#include "userstate.h"

using namespace cocos2d;
using avalon::i18n::_;

#pragma mark Initialization

HowToPlayScene::HowToPlayScene()
{
}

HowToPlayScene::~HowToPlayScene()
{
}

CCScene* HowToPlayScene::scene()
{
    CCScene *scene = CCScene::create();
    scene->addChild(HowToPlayScene::create());
    return scene;
}

bool HowToPlayScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    auto menu = CCMenu::create();
    menu->setAnchorPoint(CCPoint(0.5, 0.5));
    menu->setPosition(CCPoint(384, 120));
    addChild(menu);

    auto btn = ButtonFactory::create(
        _("menu.howto", "done").get().c_str(),
        this, menu_selector(HowToPlayScene::btnPlay)
    );
    menu->addChild(btn);

    auto text = CCLabelTTF::create(_("menu.howto", "body").get().c_str(), SMALL_FONT_NAME, 36);
    text->setAnchorPoint(CCPoint(0.5, 0.5));
    text->setPosition(CCPoint(384, 800));
    addChild(text);

    auto board = Board::create();
    board->initWithString("1rr2dr4rdul3l");
    board->enableSounds(false);
    board->playable(false);

    const float stepDelay = 1.1;
    board->runAction(
        CCRepeatForever::create(
            CCSequence::create(
                CCDelayTime::create(stepDelay),
                CCCallFunc::create(board, callfunc_selector(Board::finishTillNextCheckpoint)),
                CCDelayTime::create(stepDelay),
                CCCallFunc::create(board, callfunc_selector(Board::finishTillNextCheckpoint)),
                CCDelayTime::create(stepDelay),
                CCCallFunc::create(board, callfunc_selector(Board::finishTillNextCheckpoint)),
                CCDelayTime::create(stepDelay),
                CCCallFunc::create(board, callfunc_selector(Board::reset)),
                NULL
            )
        )
    );

    auto boardContainerInner = CCNode::create();
    boardContainerInner->setContentSize(board->getContentSize());
    boardContainerInner->setScale(BOARD_WIDTH / board->getContentSize().width);
    boardContainerInner->setAnchorPoint(CCPoint(0.5, 0.5));
    boardContainerInner->addChild(board);
    board->setTouchIndicatorScale(boardContainerInner->getScale());

    auto boardContainer = CCNode::create();
    boardContainer->setAnchorPoint(CCPoint(0.5, 0.5));
    boardContainer->setPosition(CCPoint(384, 450));
    boardContainer->addChild(boardContainerInner);
    addChild(boardContainer);

    addChild(ButtonFactory::createHeadline(_("menu.howto", "headline").get().c_str()));
    addChild(ButtonFactory::createSceneBackButton());

    auto star = ButtonFactory::createStar();
    star->enabled = false;
    addChild(star);

    return true;
}

void HowToPlayScene::btnPlay()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");

    if (globalLevel) {
        SceneManager::getInstance().gotoScene(GameScene::scene(), false);
    } else {
        SceneManager::getInstance().popScene();
    }
}