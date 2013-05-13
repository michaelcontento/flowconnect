//
//  EziSocialDefinition.h
//  EziScoailDemo
//
//  Created by Paras Mendiratta on 11/04/13.
//  EziByte (http://www.ezibyte.com)
//

/***
 
 This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 
 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 
 3. This notice may not be removed or altered from any source distribution.
 
*/



#ifndef FacebookGameDemo_EziSocialDefinition_h
#define FacebookGameDemo_EziSocialDefinition_h

#define JNI_HELPER_PATH            "../../cocos2dx/platform/android/jni/JniHelper.h"
#define CCCOMMON_PATH              "../../cocos2dx/platform/CCCommon.h"
#define FACEBOOK_PAGE_ID           "144337595753788"

#define APPCONTROLLER_HEADER       "AppController.h"
#define APPCONTROLLER_CLASS_NAME   AppController



//#define ENABLE_MAIL_FUNCTIONALITY
//#define ENABLE_TWITTER_FUNCTIONALITY

#define KEY_FB_USER_ERROR               "error"
#define KEY_FB_USER_NAME                "username"
#define KEY_FB_USER_EMAIL               "email"
#define KEY_FB_USER_GENDER              "gender"
#define KEY_FB_USER_HOMETOWN            "hometown"
#define KEY_FB_USER_PRESENT_LOCATION    "location"
#define KEY_FB_USER_PROFILE_ID          "id"
#define KEY_FB_USER_FIRST_NAME          "first_name"
#define KEY_FB_USER_LAST_NAME           "last_name"
#define KEY_FB_USER_ACCESS_TOKEN        "token"

namespace EziSocialWrapperNS
{
    
    struct FB_REQUEST
    {
        enum TYPE
        {
            REQUEST_GIFT       = 0,
            REQUEST_CHALLENGE  = 1,
            REQUEST_INVITE     = 2
        };
    };
    
    struct FB_FRIEND_SEARCH
    {
        enum TYPE
        {
            ALL_FRIENDS         = 0,
            ONLY_INSTALLED      = 1,
            ONLY_NOT_INSTALLED  = 2
        };
    };
    
    typedef void (*FBSessionCallback)(int);
    typedef void (*FBMessageCallback)(int);
    typedef void (*FBPageLikeCallback) (int);
    typedef void (*FBFriendsCallback)(const char*);
    typedef void (*FBHighScoresCallback)(const char*);
    typedef void (*FBUserDetailCallback)(const char*);
    typedef void (*MailCallback)(int);
    
    //typedef void (*FBPUserPhotoCallback)(const char*);
    
    // Request Callbacks
    typedef void (*FBSendRequestCallback)(int, const char*);
    typedef void (*FBRecieveRequestCallback)(int, const char*, const char*, const char*);
    
    
    struct FBUSER
    {
        enum PROFILE_PIC_TYPE
        {
            PIC_SQUARE      = 0,
            PIC_SMALL       = 1,
            PIC_NORMAL      = 2,
            PIC_LARGE       = 3
        };
    };
    
    struct RESPONSE_CODE
    {
        enum
        {
            // Login response code
            FB_LOGIN_NO_TOKEN           = 0,
            FB_LOGIN_SUCCESSFUL         = FB_LOGIN_NO_TOKEN     + 1,
            FB_LOGIN_FAILED             = FB_LOGIN_SUCCESSFUL   + 1,
            FB_LOGOUT_SUCCESSFUL        = FB_LOGIN_FAILED       + 1,
            
            // Message for user wall
            FB_MESSAGE_PUBLISHING_ERROR = FB_LOGOUT_SUCCESSFUL          + 1,
            FB_MESSAGE_CANCELLLED       = FB_MESSAGE_PUBLISHING_ERROR   + 1,
            FB_MESSAGE_PUBLISHED        = FB_MESSAGE_CANCELLLED         + 1,
            
            // Gift Items
            FB_GIFT_SENDING_ERROR       = FB_MESSAGE_PUBLISHED      + 1,
            FB_GIFT_SENDING_CANCELLED   = FB_GIFT_SENDING_ERROR     + 1,
            FB_GIFT_SENT                = FB_GIFT_SENDING_CANCELLED + 1,
            
            // Challenges
            FB_CHALLENGE_SENDING_ERROR  = FB_GIFT_SENT                  + 1,
            FB_CHALLEGE_CANCELLED       = FB_CHALLENGE_SENDING_ERROR    + 1,
            FB_CHALLEGE_SENT            = FB_CHALLEGE_CANCELLED         + 1,
            
            // Invite
            FB_INVITE_SENDING_ERROR     = FB_CHALLEGE_SENT          + 1,
            FB_INVITE_CANCELLED         = FB_INVITE_SENDING_ERROR   + 1,
            FB_INVITE_SENT              = FB_INVITE_CANCELLED       + 1,
            
            
            // Page Like
            FB_PAGELIKE_ERROR           = FB_INVITE_SENT        + 1,
            FB_PAGELIKE_NEGATIVE        = FB_PAGELIKE_ERROR     + 1,
            FB_PAGELIKE_POSITIVE        = FB_PAGELIKE_NEGATIVE  + 1,
            
            // Email Response Code
            MAIL_SEND_ERROR             = FB_PAGELIKE_POSITIVE  + 1,
            MAIL_SEND_SUCCESS           = MAIL_SEND_ERROR       + 1,
            MAIL_SEND_CANCLLED          = MAIL_SEND_SUCCESS     + 1,
            MAIL_SEND_STORED            = MAIL_SEND_CANCLLED    + 1,
            
            // Recieve
            FB_REQUEST_RECEIVE_PARSING_ERROR   = MAIL_SEND_STORED                 + 1,
            FB_REQUEST_RECEIVE_SESSION_ERROR   = FB_REQUEST_RECEIVE_PARSING_ERROR + 1,
            
            FB_CHALLENGE_RECEIVE  = FB_REQUEST_RECEIVE_SESSION_ERROR + 1,
            FB_GIFT_RECEIVE       = FB_CHALLENGE_RECEIVE             + 1,
            FB_INVITE_RECEIVE     = FB_GIFT_RECEIVE                  + 1
            
            
            
        };
    };
    
    
    // Messages
    void postMessage(FBMessageCallback callback,
                     const char* heading,
                     const char* caption,
                     const char* description,
                     const char* badgeIconURL,
                     const char* deepLinkURL);
    
    void autoPostMessageOnWall(FBMessageCallback callback,
                               const char* heading,
                               const char* caption,
                               const char* description,
                               const char* badgeIconURL,
                               const char* deepLinkURL);
    
    
    void postScore(EziSocialWrapperNS::FBMessageCallback callback,
                   unsigned long long score);
    
    // User details
    void fetchUserDetails(FBUserDetailCallback callback, bool getEmailIDAlso);
    
    // User login / logout
    void loginWithFacebook(FBSessionCallback callback);
    void logoutFromFacebook(FBSessionCallback callback);
    
    // Check if user has liked my page
    void hasUserLikePage(FBPageLikeCallback callback, const char*pageID);
    
    // Get friends
    void getFriends(FBFriendsCallback callback, FB_FRIEND_SEARCH::TYPE searchType, int startIndex, int limit);
    
    // Get the high score of user and his friends.
    void getHighScores(FBHighScoresCallback callback);
    
    // Open Facebook Page and on returing back to application, check if user likes Facebook page.
    void openFacebookPage(const char* pageID, bool checkPageLike, FBPageLikeCallback callback);
    
    
    void sendRequest(FBSendRequestCallback callback,
                     FB_REQUEST::TYPE requestType,
                     const char* message,
                     const char* dataDictionary,
                     const char* preselectedFriends);
    
    void setRequestRecievedCallback(FBRecieveRequestCallback callback);
    
    bool isFacebookSessionActive();
    
    // Twitter Methods...
    void tweet(const char* message, const char* imageURL);
    
    // Email Methods
    void sendEmail(const char* subject, const char* messageBody, const char* recipents, MailCallback callback);
    
    // Network Status
    bool networkAvailableForHost(const char* hostURL);
    
    
}
#endif
