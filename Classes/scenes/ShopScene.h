#ifndef __FlowConnect__ShopScene__
#define __FlowConnect__ShopScene__

#include <avalon/payment.h>
#include "cocos2d.h"
#include "../Alert.h"
#include "EziSocialDelegate.h"

class ShopScene
: public cocos2d::CCLayer
, public avalon::payment::ManagerDelegate
, public EziFacebookDelegate
{
public:
    ShopScene();
    virtual ~ShopScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(ShopScene);
    virtual bool init() override;

    void btnFacebookLike();
    void btnRateUs();

    virtual void fbSessionCallback(int responseCode);
    virtual void fbUserDetailCallback(cocos2d::CCDictionary* data);
    virtual void fbMessageCallback(int responseCode);
    virtual void fbPageLikeCallback(int responseCode);
    virtual void fbFriendsCallback(cocos2d::CCArray* friends);
    virtual void fbHighScoresCallback(cocos2d::CCArray* highScores);
    virtual void fbSendRequestCallback(int responseCode, cocos2d::CCArray* friendsGotRequests);
    virtual void fbRecieveRequestCallback(int responseCode, const char* message, const char* senderName, cocos2d::CCDictionary* dataDictionary);
    virtual void fbUserPhotoCallback(const char *userPhotoPath);

    void alertTimeout(CCObject* alert);

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
