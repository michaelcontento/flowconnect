#import "GameCenterLauncher.h"

#import <GameKit/GameKit.h>
#import "AppController.h"
#import "RootViewController.h"

@implementation GameCenterLauncher

+(void)login
{
    GKLocalPlayer* localPlayer = [GKLocalPlayer localPlayer];
    [localPlayer authenticateWithCompletionHandler:^(NSError *error) {
        if (error) {
            NSLog(@"[GameCenter] login failed: %@", error.localizedDescription);
        }
        // The result of this method call is simply ignored. But if your curious
        // how to use it: if (localPlayer.authenticated) { /* .. */ }
        //
        // Why is the result ignored? Because this method will open up the
        // register/login dialog -- OR -- authenticate the user and don't
        // show anything.
        //
        // All other methods do the "is the user logged in?"-check at their own
        // so we don't have anything left to do here.
    }];
}

+(void)openAchievement
{
    GKLocalPlayer* localPlayer = [GKLocalPlayer localPlayer];
    if (!localPlayer.authenticated) {
        [self showNotAuthenticatedDialog];
        return;
    }

    AppController* appController = (AppController*) [UIApplication sharedApplication].delegate;

    GKAchievementViewController* achievementController = [[GKAchievementViewController alloc] init];
    achievementController.achievementDelegate = appController->viewController;

    [appController->viewController presentModalViewController:achievementController
                                                     animated:YES];
}

+(void)postAchievement:(const char*)idName percent:(NSNumber*)percentComplete
{
    GKLocalPlayer* localPlayer = [GKLocalPlayer localPlayer];
    if (![localPlayer isAuthenticated]) {
        NSLog(@"[GameCenter] postAchievement failed: User is not authenticated.");
        return;
    }

    GKAchievement* achievement = [[GKAchievement alloc] initWithIdentifier:[NSString stringWithUTF8String:idName]];
    achievement.percentComplete = [percentComplete doubleValue];
    achievement.showsCompletionBanner = YES;

    [achievement reportAchievementWithCompletionHandler:^(NSError* error) {
        if (error) {
            NSLog(@"[GameCenter] postAchievement failed: %@", error.localizedDescription);
        }
    }];
}

+(void)openLeaderboard
{
    GKLocalPlayer* localPlayer = [GKLocalPlayer localPlayer];
    if (!localPlayer.authenticated) {
        [self showNotAuthenticatedDialog];
        return;
    }

    AppController* appController = (AppController*) [UIApplication sharedApplication].delegate;

    GKLeaderboardViewController* leaderboardController = [[GKLeaderboardViewController alloc] init];
    leaderboardController.timeScope = GKLeaderboardTimeScopeAllTime;
    leaderboardController.leaderboardDelegate = appController->viewController;

    [appController->viewController presentModalViewController:leaderboardController
                                                     animated:YES];
}

+(void)postScore:(const char*)idName score:(NSNumber*)score;
{
    GKLocalPlayer* localPlayer = [GKLocalPlayer localPlayer];
    if (![localPlayer isAuthenticated]) {
        NSLog(@"[GameCenter] postScore failed: User is not authenticated.");
        return;
    }

    GKScore* gkScore = [[GKScore alloc] initWithCategory:[NSString stringWithUTF8String:idName]];
    gkScore.value = [score longLongValue];
    gkScore.shouldSetDefaultLeaderboard = YES;

    // TODO: Can this be removed?
    gkScore.context = 1;

    [gkScore reportScoreWithCompletionHandler:^(NSError* error) {
        if (error) {
            NSLog(@"[GameCenter] postScore failed: %@", error.localizedDescription);
        }
    }];
}

// TODO: This method should be private
+(void)showNotAuthenticatedDialog
{
    // TODO: i18n this thing!
    NSString* alertTitle = @"";
    NSString* alertMessage = @"You must logged in to Game Center.";
    UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:alertTitle
                                                        message:alertMessage
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
    [alertView autorelease];
    [alertView show];
}

@end