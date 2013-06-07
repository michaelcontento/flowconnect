#include "ShopScene.h"

#include <boost/cast.hpp>
#include <avalon/i18n/LanguageKey.h>
#include <avalon/i18n/Localization.h>
#include <avalon/utils/url.h>
#include <avalon/utils/platform.h>
#include <avalon/ads/Manager.h>
#include "../buttons/ButtonFactory.h"
#include "../Alert.h"
#include "userstate.h"
#include "SceneManager.h"

using namespace cocos2d;
using namespace avalon;
using boost::polymorphic_downcast;
using avalon::i18n::_;

#pragma mark Initialization

ShopScene::ShopScene()
: menu(NULL)
{
    auto manager = payment::Loader::globalManager;
    manager->delegate = this;
}

ShopScene::~ShopScene()
{
    auto manager = payment::Loader::globalManager;
    manager->delegate = NULL;
}

CCScene* ShopScene::scene()
{
    CCScene *scene = CCScene::create();
    scene->addChild(ShopScene::create());
    return scene;
}

void ShopScene::onEnter()
{
    CCLayer::onEnter();
    avalon::ads::Manager::showBannerIgnoreTime();
}

bool ShopScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    auto manager = payment::Loader::globalManager;
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

    setKeypadEnabled(true);
    return true;
}

void ShopScene::keyBackClicked()
{
    SceneManager::getInstance().popSceneWithSound();
}


void ShopScene::createMenu(payment::Manager* manager)
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

    auto manager = payment::Loader::globalManager;
    if (!manager->isPurchaseReady()) {
        manager->stopService();
        SceneManager::getInstance().popScene();
    }
}

void ShopScene::onServiceStarted(payment::Manager* const manager)
{
    createMenu(manager);
    showSpinner(false);
}

void ShopScene::onPurchaseSucceed(payment::Manager *const manager, payment::Product *const product)
{
    auto consumable = boost::polymorphic_downcast<payment::ProductConsumable*>(product);

    userstate::addStarsToUser(consumable->getQuantity());
    consumable->consume();
}

void ShopScene::onPurchaseFail(payment::Manager* const manager)
{
    CCMessageBox(
        _("dialog.shoppurchaseerror", "body").get().c_str(),
        _("dialog.shoppurchaseerror", "headline").get().c_str()
    );
}

void ShopScene::onTransactionStart(payment::Manager *const manager)
{
    retain();
    showSpinner(true);
}

void ShopScene::onTransactionEnd(payment::Manager *const manager)
{
    showSpinner(false);
    release();
}

void ShopScene::btnFacebookLike()
{
    avalon::utils::url::open("https://www.facebook.com/coragames");

    userstate::fbLike();
    CCMessageBox(
        _("dialog.fblikethx", "body").assign("amount", FREE_STARS).get().c_str(),
        _("dialog.fblikethx", "headline").get().c_str()
    );

    menu->removeChildByTag(tagFbLike);
    menu->alignItemsVerticallyWithPadding(MENU_PADDING);
}

void ShopScene::btnRateUs()
{
#if AVALON_PLATFORM_IS_IOS
    avalon::utils::url::open("itms-apps://itunes.apple.com/app/id/643074518");
#elif AVALON_PLATFORM_IS_ANDROID_AMAZON
    avalon::utils::url::open("amzn://apps/android?p=com.coragames.dtdng");
#elif AVALON_PLATFORM_IS_ANDROID_GOOGLE
    avalon::utils::url::open("market://details?id=com.coragames.dtdng");
#elif AVALON_PLATFORM_IS_ANDROID_SAMSUNG
    avalon::utils::url::open("samsungapps://ProductDetail/com.coragames.dtdng");
#endif

    userstate::rateUs();
    CCMessageBox(
        _("dialog.rateusthx", "body").assign("amount", FREE_STARS).get().c_str(),
        _("dialog.rateusthx", "headline").get().c_str()
    );

    menu->removeChildByTag(tagRateUs);
    menu->alignItemsVerticallyWithPadding(MENU_PADDING);
}