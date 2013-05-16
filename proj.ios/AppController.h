//
//  iphoneAppDelegate.h
//  iphone
//
//  Created by Walzer on 10-11-16.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#define WITH_LOCALYTICS 1
#define WITH_CRASHLYTICS 1

#import <UIKit/UIKit.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    
    @public
    RootViewController    *viewController;
}

@end

