#ifndef __FlowConnect__ShopScene__
#define __FlowConnect__ShopScene__

#include <avalon/payment.h>
#include "cocos2d.h"

class ShopScene
: public cocos2d::CCLayer
, public avalon::payment::ManagerDelegate
{
public:
    ShopScene();
    virtual ~ShopScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(ShopScene);
    virtual bool init() override;

    void btnFacebookLike();
    void btnRateUs();

    void alertTimeout(CCObject* alert);
    virtual void keyBackClicked() override;

    /* Payment Receiver Interface */
    void onServiceStarted(avalon::payment::Manager* const manager);
    void onPurchaseSucceed(avalon::payment::Manager* const manager, avalon::payment::Product* const product);
    void onPurchaseFail(avalon::payment::Manager* const manager);
    void onTransactionStart(avalon::payment::Manager* const manager);
    void onTransactionEnd(avalon::payment::Manager* const manager);
    
private:
    static const unsigned int MENU_PADDING = 25;
    static const int tagAlert = 10;
    static const int tagRateUs = 11;
    static const int tagFbLike = 12;
    static const int shopTimeout = 15;
    cocos2d::CCMenu* menu;

    void createMenu(avalon::payment::Manager* manager);
    void showSpinner(const bool flag);
};

#endif /* defined(__FlowConnect__ShopScene__) */
