#include "ShopScene.h"

#include "ButtonFactory.h"
#include "userstate.h"
#include "Localization.h"
#include "LanguageKey.h"

using namespace cocos2d;

#pragma mark Initialization

ShopScene::ShopScene()
{
}

ShopScene::~ShopScene()
{
}

CCScene* ShopScene::scene()
{
    CCScene *scene = CCScene::create();
    scene->addChild(ShopScene::create());
    return scene;
}

bool ShopScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    auto menu = CCMenu::create();
    addChild(menu);

    menu->addChild(ButtonFactory::create("100 Stars 2€", this, menu_selector(ShopScene::btnPurchase)));
    menu->addChild(ButtonFactory::create("300 Stars 5€", this, menu_selector(ShopScene::btnPurchase)));
    menu->addChild(ButtonFactory::create("600 + 100 Stars 10€", this, menu_selector(ShopScene::btnPurchase)));
    menu->addChild(ButtonFactory::create("1200 + 400 Stars 20€", this, menu_selector(ShopScene::btnPurchase)));
    menu->addChild(ButtonFactory::create("1800 + 900 Stars 30€", this, menu_selector(ShopScene::btnPurchase)));
    menu->addChild(ButtonFactory::create("Rate us - 10", this, menu_selector(ShopScene::btnPurchase)));
    menu->addChild(ButtonFactory::create("Facebook like - 10", this, menu_selector(ShopScene::btnPurchase)));
    menu->alignItemsVerticallyWithPadding(MENU_PADDING);

    addChild(ButtonFactory::createHeadline(_("menu.shop", "headline")->getCString()));
    addChild(ButtonFactory::createSceneBackButton());

    auto star = ButtonFactory::createStar();
    star->enabled = false;
    addChild(star);

    return true;
}

void ShopScene::btnPurchase()
{
    userstate::addStarsToUser(100);
}