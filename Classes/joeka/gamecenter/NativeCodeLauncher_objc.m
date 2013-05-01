#import "NativeCodeLauncher_objc.h"

#import "RootViewController.h"
#import "AppController.h"
#import <GameKit/GameKit.h>

@implementation NativeCodeLauncher

// Log in to check GameCenter
+(void)loginGameCenter
{
        GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
        [localPlayer authenticateWithCompletionHandler:^(NSError *error) {
                if(localPlayer.authenticated)
                {

                }
                else
                {

                }
        }];
}

// I open the Leaderboard
+(void)openRanking
{
        GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
        if(localPlayer.authenticated)
        {
                AppController *appController = (AppController *)[UIApplication sharedApplication].delegate;

                GKLeaderboardViewController *leaderboardController = [[GKLeaderboardViewController alloc]init];
                leaderboardController.timeScope = GKLeaderboardTimeScopeAllTime;
                leaderboardController.leaderboardDelegate = appController->viewController;

                [appController->viewController presentModalViewController:leaderboardController animated:YES];
        }
        else
        {
                NSString *alertTitle = @"";
                NSString *alertMessage = @"You must logged in to Game Center.";
                UIAlertView *alertView = [[UIAlertView alloc]initWithTitle:alertTitle message:alertMessage
                                                                                                                  delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
                [alertView show];
        }
}

// Submit score to the Leaderboard
+(void)postHighScore:(const char *)idName score:(NSNumber *)score;
{
        GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
        if([localPlayer isAuthenticated])
        {
                GKScore *gkScore = [[GKScore alloc]initWithCategory:[NSString stringWithUTF8String:idName]];
                gkScore.value = [score longLongValue];
                gkScore.context = 1;
                [gkScore reportScoreWithCompletionHandler:^(NSError *error) {
                        if(error)
                        {
                                NSLog(@"Error : %@",error);
                        }
                        else
                        {
                                NSLog(@"Sent highscore.");
                        }
                }];
        }
        else
        {
                NSLog(@"Gamecenter not authenticated.");
        }
}

// I open the Achievement
+(void)openAchievement
{
        GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
        if(localPlayer.authenticated)
        {
                AppController *appController = (AppController *) [UIApplication sharedApplication].delegate;

                GKAchievementViewController *achievementController = [[GKAchievementViewController alloc]init];
                achievementController.achievementDelegate = appController->viewController;

                [appController->viewController presentModalViewController:achievementController animated:YES];

        }
        else
        {
                NSString *alertTitle = @"";
                NSString *alertMessage = @"You must logged in to Game Center.";
                UIAlertView *alertView = [[UIAlertView alloc]initWithTitle:alertTitle message:alertMessage
                                                                                                                  delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
                [alertView show];
        }
}

// Send achievement of achievement items
+(void)postAchievement:(const char *)idName percent:(NSNumber *)percentComplete
{
        GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
        if([localPlayer isAuthenticated])
        {
                GKAchievement *achievement = [[GKAchievement alloc] initWithIdentifier:[NSString stringWithUTF8String:idName]];
                achievement.percentComplete = [percentComplete doubleValue];
                achievement.showsCompletionBanner = YES;
                [achievement reportAchievementWithCompletionHandler:^(NSError *error) {
                        if(error)
                        {
                                NSLog(@"Error : %@",error);
                        }
                        else
                        {
                                NSLog(@"Sent Achievement.");
                        }
                }];
        }
        else
        {
                NSLog(@"Gamecenter not authenticated.");
        }
}

@end
