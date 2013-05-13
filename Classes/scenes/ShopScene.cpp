#include "ShopScene.h"

#include <boost/cast.hpp>
#include "ButtonFactory.h"
#include "userstate.h"
#include "Localization.h"
#include "LanguageKey.h"
#include "SceneManager.h"
#include "UrlOpener.h"

#ifdef WITH_EZISOCIAL
#include "EziSocialObject.h"
#endif

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

    addChild(ButtonFactory::createHeadline(_("menu.shop", "headline").get().c_str()));
    addChild(ButtonFactory::createSceneBackButton());

    auto star = ButtonFactory::createStar();
    star->enabled = false;
    addChild(star);

    #ifdef WITH_EZISOCIAL
    if (!userstate::fbLikeDone()) {
        EziSocialObject::sharedObject()->setFacebookDelegate(this);
    }
    #endif

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
    if (!userstate::rateUsDone()) {
        auto btn = ButtonFactory::createPaymentButton(
            _("shop.rateus", "name").assign("amount", FREE_STARS).get().c_str(),
            _("shop.rateus", "desc").assign("amount", FREE_STARS).get().c_str(),
            _("shop.rateus", "price").assign("amount", FREE_STARS).get().c_str(),
            this, menu_selector(ShopScene::btnRateUs)
        );
        btn->setTag(tagRateUs);
        menu->addChild(btn);
    }
    if (!userstate::fbLikeDone()) {
        auto btn = ButtonFactory::createPaymentButton(
            _("shop.fblike", "name").assign("amount", FREE_STARS).get().c_str(),
            _("shop.fblike", "desc").assign("amount", FREE_STARS).get().c_str(),
            _("shop.fblike", "price").assign("amount", FREE_STARS).get().c_str(),
            this, menu_selector(ShopScene::btnFacebookLike)
        );
        btn->setTag(tagFbLike);
        menu->addChild(btn);
    }
    menu->alignItemsVerticallyWithPadding(MENU_PADDING);
}

void ShopScene::showSpinner(const bool flag)
{
    if (menu) {
        menu->setEnabled(!flag);
    }

    if (flag) {
        auto alert = Alert::create();
        alert->setHeadline(_("dialog.shopaction", "headline").get().c_str());
        alert->setBody(_("dialog.shopaction", "body").get().c_str());
        alert->showSpinner();
        alert->setTag(tagAlert);
        alert->setTimeout(shopTimeout, this, callfuncN_selector(ShopScene::alertTimeout));
        addChild(alert);
    } else {
        removeChildByTag(tagAlert);
    }
}

void ShopScene::alertTimeout(CCObject* alert)
{
    showSpinner(false);    
    CCMessageBox(
        _("dialog.shoptimeouterror", "body").get().c_str(),
        _("dialog.shoptimeouterror", "headline").get().c_str()
    );

    auto manager = Payment::Loader::globalManager;
    if (!manager->isPurchaseReady()) {
        manager->stopService();
        SceneManager::getInstance().popScene();
    }
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
        _("dialog.shoppurchaseerror", "body").get().c_str(),
        _("dialog.shoppurchaseerror", "headline").get().c_str()
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

void ShopScene::btnFacebookLike()
{
    #ifdef WITH_EZISOCIAL
    EziSocialObject::sharedObject()->openFacebookPage("212046412247647", true);
    #else
    fbPageLikeCallback(-1);
    #endif
}

void ShopScene::btnRateUs()
{
    UrlOpener::open("itms-apps://itunes.apple.com/app/id/643074518");
    userstate::rateUs();
    CCMessageBox(
        _("dialog.rateusthx", "body").assign("amount", FREE_STARS).get().c_str(),
        _("dialog.rateusthx", "headline").get().c_str()
    );

    menu->removeChildByTag(tagRateUs);
    menu->alignItemsVerticallyWithPadding(MENU_PADDING);
}

void ShopScene::fbSessionCallback(int responseCode)
{
}

void ShopScene::fbUserDetailCallback(cocos2d::CCDictionary* data)
{
}

void ShopScene::fbMessageCallback(int responseCode)
{
}

void ShopScene::fbPageLikeCallback(int responseCode)
{
    userstate::fbLike();
    CCMessageBox(
        _("dialog.fblikethx", "body").assign("amount", FREE_STARS).get().c_str(),
        _("dialog.fblikethx", "headline").get().c_str()
    );

    menu->removeChildByTag(tagFbLike);
    menu->alignItemsVerticallyWithPadding(MENU_PADDING);
}

void ShopScene::fbFriendsCallback(cocos2d::CCArray* friends)
{
}

void ShopScene::fbHighScoresCallback(cocos2d::CCArray* highScores)
{
}

void ShopScene::fbSendRequestCallback(int responseCode, cocos2d::CCArray* friendsGotRequests)
{
}

void ShopScene::fbRecieveRequestCallback(int responseCode, const char* message, const char* senderName, cocos2d::CCDictionary* dataDictionary)
{
}

void ShopScene::fbUserPhotoCallback(const char *userPhotoPath)
{
}