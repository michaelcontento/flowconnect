//
//  EziSocialDelegate.h
//  EziSocialDemo
//
//  Created by Paras Mendiratta on 15/04/13.
//  EziByte (http://www.ezibyte.com)
//

/***
 
 This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 
 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 
 3. This notice may not be removed or altered from any source distribution.
 
*/



#ifndef FacebookGameDemo_EziSocialDelegate_h
#define FacebookGameDemo_EziSocialDelegate_h

#include "cocos2d.h"

class EziFacebookDelegate
{
public:
    virtual void fbSessionCallback(int responseCode) = 0;
    virtual void fbUserDetailCallback(cocos2d::CCDictionary* data) = 0;
    virtual void fbMessageCallback(int responseCode) = 0;
    
    virtual void fbSendRequestCallback(int responseCode, cocos2d::CCArray* friendsGotRequests) = 0;
    virtual void fbRecieveRequestCallback(int responseCode,
                                          const char* message,
                                          const char* senderName, cocos2d::CCDictionary* dataDictionary) = 0;
    
    virtual void fbPageLikeCallback(int responseCode) = 0;
    virtual void fbFriendsCallback(cocos2d::CCArray* friends) = 0;
    virtual void fbHighScoresCallback(cocos2d::CCArray* highScores) = 0;
    
    virtual void fbUserPhotoCallback(const char *userPhotoPath) = 0;
};

class EziEmailDelegate
{
public:
    virtual void mailCallback(int responseCode) = 0;
};

class EziTwitterDelegate
{
    
public:
    virtual void twSessionCallback(int responseCode) = 0;
};

#endif
