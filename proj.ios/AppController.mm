//
//  iphoneAppDelegate.m
//  iphone
//
//  Created by Walzer on 10-11-16.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "AppController.h"
#import "EAGLView.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#include "userstate.h"
#import "EziSocialManager.h"

#ifdef WITH_LOCALYTICS
#import "LocalyticsSession.h"
#endif

#ifdef WITH_CRASHLYTICS
#import <Crashlytics/Crashlytics.h>
#endif

@implementation AppController

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
     
    // Init the EAGLView
    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGB565
                                     depthFormat: GL_DEPTH24_STENCIL8_OES
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples: 0];
    [__glView setMultipleTouchEnabled:YES];

    // Use RootViewController manage EAGLView 
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    viewController.view = __glView;

    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:viewController];
    }
    
    [window makeKeyAndVisible];

    [[UIApplication sharedApplication] setStatusBarHidden:true];

    cocos2d::CCApplication::sharedApplication()->run();

    #ifdef WITH_CRASHLYTICS
    [Crashlytics startWithAPIKey:@"0bba8db2fa145bc487dc41da3d3cff39d062166d"];
    #endif

    #ifdef WITH_LOCALYTICS
    [[LocalyticsSession shared] startSession:@"ee497b7262c9e2c79ee9bc8-f09416dc-b981-11e2-895c-005cf8cbabd8"];
    #endif

    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    cocos2d::CCDirector::sharedDirector()->pause();

    #ifdef WITH_LOCALYTICS
    NSDictionary *dictionary = [NSDictionary dictionaryWithObjectsAndKeys:
        [[NSString alloc] initWithFormat:@"%d", userstate::getStarsReporting()],
        @"stars",
         nil];
    [[LocalyticsSession shared] tagEvent:@"stats" attributes:dictionary];

    [[LocalyticsSession shared] close];
    [[LocalyticsSession shared] upload];
    #endif
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    cocos2d::CCDirector::sharedDirector()->resume();

    [[EziSocialManager sharedManager] handleApplicationDidBecomeActive];

    #ifdef WITH_LOCALYTICS
    [[LocalyticsSession shared] resume];
    [[LocalyticsSession shared] upload];
    #endif
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    if ([sourceApplication isEqualToString:@"com.apple.mobilesafari"] ||
        [sourceApplication isEqualToString:@"com.facebook.Facebook"]) {
        return [[EziSocialManager sharedManager] handleURL:url];
    } else {
        return NO;
    }
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();

    #ifdef WITH_LOCALYTICS
    NSDictionary *dictionary = [NSDictionary dictionaryWithObjectsAndKeys:
        [[NSString alloc] initWithFormat:@"%d", userstate::getStarsReporting()],
        @"stars",
        nil];
    [[LocalyticsSession shared] tagEvent:@"stats" attributes:dictionary];

    [[LocalyticsSession shared] close];
    [[LocalyticsSession shared] upload];
    #endif
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();

    #ifdef WITH_LOCALYTICS
    [[LocalyticsSession shared] resume];
    [[LocalyticsSession shared] upload];
    #endif
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */

    #ifdef WITH_LOCALYTICS
    NSDictionary *dictionary = [NSDictionary dictionaryWithObjectsAndKeys:
        [[NSString alloc] initWithFormat:@"%d", userstate::getStarsReporting()],
        @"stars",
        nil];
    [[LocalyticsSession shared] tagEvent:@"stats" attributes:dictionary];
     
    [[LocalyticsSession shared] close];
    [[LocalyticsSession shared] upload];
    #endif
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc {
    [window release];
    [super dealloc];
}


@end
