#include "ShopScene.h"

#include <boost/cast.hpp>
#include "ButtonFactory.h"
#include "userstate.h"
#include "Localization.h"
#include "LanguageKey.h"
#include "Product.h"
#include "ProductConsumable.h"
#include "Loader.h"
#include "Manager.h"

using namespace cocos2d;
using namespace Avalon;
using boost::polymorphic_downcast;

#pragma mark Initialization

ShopScene::ShopScene()
: menu(NULL)
{
    auto manager = Payment::Loader::globalManager;
    manager->delegate = this;
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

    auto manager = Payment::Loader::globalManager;
    if (manager->isPurchaseReady()) {
        createMenu(manager.get());
    } else {
        if (!manager->isStarted()) {
            manager->startService();
        }
        showSpinner(true);
    }

    addChild(ButtonFactory::createHeadline(_("menu.shop", "headline")->getCString()));
    addChild(ButtonFactory::createSceneBackButton());

    auto star = ButtonFactory::createStar();
    star->enabled = false;
    addChild(star);

    return true;
}

void ShopScene::createMenu(Payment::Manager* manager)
{
    menu = CCMenu::create();
    addChild(menu);

    menu->addChild(ButtonFactory::createPaymentButton(manager->getProduct("100")));
    menu->addChild(ButtonFactory::createPaymentButton(manager->getProduct("300")));
    menu->addChild(ButtonFactory::createPaymentButton(manager->getProduct("700")));
    menu->addChild(ButtonFactory::createPaymentButton(manager->getProduct("1600")));
    menu->addChild(ButtonFactory::createPaymentButton(manager->getProduct("2700")));
    //menu->addChild(ButtonFactory::create("Rate us - 10", this, menu_selector(ShopScene::btnPurchase)));
    //menu->addChild(ButtonFactory::create("Facebook like - 10", this, menu_selector(ShopScene::btnPurchase)));
    menu->alignItemsVerticallyWithPadding(MENU_PADDING);

}

void ShopScene::showSpinner(const bool flag)
{
    if (menu) {
        menu->setEnabled(!flag);
    }

    // TODO: Implement spinner
}

void ShopScene::onServiceStarted(Payment::Manager* const manager)
{
    createMenu(manager);
    showSpinner(false);
}

void ShopScene::onPurchaseSucceed(Payment::Manager *const manager, Payment::Product *const product)
{
    auto consumable = boost::polymorphic_downcast<Payment::ProductConsumable*>(product);

    userstate::addStarsToUser(consumable->getQuantity());
    consumable->consume();
}

void ShopScene::onPurchaseFail(Payment::Manager* const manager)
{
    CCMessageBox(
        _("dialog.shoperror", "body")->getCString(),
        _("dialog.shoperror", "headline")->getCString()
    );
}

void ShopScene::onTransactionStart(Payment::Manager *const manager)
{
    retain();
    showSpinner(true);
}

void ShopScene::onTransactionEnd(Payment::Manager *const manager)
{
    showSpinner(false);
    release();
}