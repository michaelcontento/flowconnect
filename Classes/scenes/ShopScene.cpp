#include "ShopScene.h"

#include "ButtonFactory.h"
#include "userstate.h"
#include "Localization.h"
#include "LanguageKey.h"
#include "Product.h"
#include "ProductConsumable.h"
#include "PaymentService.h"

using namespace cocos2d;

#pragma mark Initialization

ShopScene::ShopScene()
: menu(NULL)
, purchased(false)
{
    PaymentService::setPaymentReceiver(this);
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

    menu = CCMenu::create();
    addChild(menu);

    menu->addChild(ButtonFactory::createPaymentButton(PaymentService::getProduct("100")));
    menu->addChild(ButtonFactory::createPaymentButton(PaymentService::getProduct("300")));
    menu->addChild(ButtonFactory::createPaymentButton(PaymentService::getProduct("700")));
    menu->addChild(ButtonFactory::createPaymentButton(PaymentService::getProduct("1600")));
    menu->addChild(ButtonFactory::createPaymentButton(PaymentService::getProduct("2700")));
    //menu->addChild(ButtonFactory::create("Rate us - 10", this, menu_selector(ShopScene::btnPurchase)));
    //menu->addChild(ButtonFactory::create("Facebook like - 10", this, menu_selector(ShopScene::btnPurchase)));
    menu->alignItemsVerticallyWithPadding(MENU_PADDING);

    addChild(ButtonFactory::createHeadline(_("menu.shop", "headline")->getCString()));
    addChild(ButtonFactory::createSceneBackButton());

    auto star = ButtonFactory::createStar();
    star->enabled = false;
    addChild(star);

    return true;
}

void ShopScene::onPurchaseStateChanged(Product *product)
{
    purchased = true;
    auto desc = product->nativeId;

    auto pos = desc.find_last_of(".");
    if (pos == std::string::npos) {
        return;
    }

    auto amount = atoi(desc.substr(pos + 1, std::string::npos).c_str());
    userstate::addStarsToUser(amount);
}

void ShopScene::onTransactionStart()
{
    retain();
    purchased = false;

    if (menu) {
        menu->setEnabled(false);
    }
}

void ShopScene::onTransactionStop()
{
    if (menu) {
        menu->setEnabled(true);
    }

    if (!purchased) {
        CCMessageBox(
            _("dialog.shoperror", "body")->getCString(),
            _("dialog.shoperror", "headline")->getCString()
        );
    }

    release();
}