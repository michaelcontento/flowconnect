#ifndef __FlowConnect__ShopScene__
#define __FlowConnect__ShopScene__

#include "cocos2d.h"

class ShopScene : public cocos2d::CCLayer
{
public:
    ShopScene();
    virtual ~ShopScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(ShopScene);
    virtual bool init() override;

    void btnPurchase();

private:
    const unsigned int MENU_PADDING = 25;
};

#endif /* defined(__FlowConnect__ShopScene__) */
