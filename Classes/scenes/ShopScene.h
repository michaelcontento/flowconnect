#ifndef __FlowConnect__ShopScene__
#define __FlowConnect__ShopScene__

#include "cocos2d.h"
#include "PaymentListener.h"

class ShopScene : public cocos2d::CCLayer, public PaymentListener
{
public:
    ShopScene();
    virtual ~ShopScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(ShopScene);
    virtual bool init() override;

    /* Payment Receiver Interface */
    virtual void onPurchaseStateChanged(Product *p);
    virtual void onTransactionStart();
    virtual void onTransactionStop();

private:
    const unsigned int MENU_PADDING = 25;
    cocos2d::CCMenu* menu;
    bool purchased;
};

#endif /* defined(__FlowConnect__ShopScene__) */
