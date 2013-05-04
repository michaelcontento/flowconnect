#ifndef __FlowConnect__ShopScene__
#define __FlowConnect__ShopScene__

#include "cocos2d.h"
#include "../joeka/Payment.h"

class ShopScene : public cocos2d::CCLayer, public Avalon::Payment::ManagerDelegate
{
public:
    ShopScene();
    virtual ~ShopScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(ShopScene);
    virtual bool init() override;

    /* Payment Receiver Interface */
    void onServiceStarted(Avalon::Payment::Manager* const manager);

    void onPurchaseSucceed(Avalon::Payment::Manager* const manager, Avalon::Payment::Product* const product);
    void onPurchaseFail(Avalon::Payment::Manager* const manager);

    void onTransactionStart(Avalon::Payment::Manager* const manager);
    void onTransactionEnd(Avalon::Payment::Manager* const manager);
    
private:
    const unsigned int MENU_PADDING = 25;
    cocos2d::CCMenu* menu;

    void createMenu(Avalon::Payment::Manager* manager);
    void showSpinner(const bool flag);
};

#endif /* defined(__FlowConnect__ShopScene__) */
