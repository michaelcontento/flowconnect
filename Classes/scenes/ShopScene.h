#ifndef __FlowConnect__ShopScene__
#define __FlowConnect__ShopScene__

#include "cocos2d.h"
#include "../joeka/Payment.h"
#include "../Alert.h"
#include "EziSocialDelegate.h"

class ShopScene
: public cocos2d::CCLayer
, public Avalon::Payment::ManagerDelegate
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
    void onServiceStarted(Avalon::Payment::Manager* const manager);

    void onPurchaseSucceed(Avalon::Payment::Manager* const manager, Avalon::Payment::Product* const product);
    void onPurchaseFail(Avalon::Payment::Manager* const manager);

    void onTransactionStart(Avalon::Payment::Manager* const manager);
    void onTransactionEnd(Avalon::Payment::Manager* const manager);
    
private:
    const unsigned int MENU_PADDING = 25;
    const int tagAlert = 10;
    const int tagRateUs = 11;
    const int tagFbLike = 12;
    const float shopTimeout = 15;
    cocos2d::CCMenu* menu;

    void createMenu(Avalon::Payment::Manager* manager);
    void showSpinner(const bool flag);
};

#endif /* defined(__FlowConnect__ShopScene__) */
