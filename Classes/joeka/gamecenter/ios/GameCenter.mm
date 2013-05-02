#import "GameCenter.h"
#import "GameCenterLauncher.h"

void GameCenter::login()
{
    [[GameCenterLauncher shared] login];
}

#pragma mark -
#pragma mark Achievements

void GameCenter::openAchievement()
{
    [[GameCenterLauncher shared] openAchievement];
}

void GameCenter::postAchievement(const char* idName, int percentComplete)
{
    [[GameCenterLauncher shared] postAchievement:idName percent:[NSNumber numberWithInt:percentComplete]];
}

void GameCenter::clearAllAchievements()
{
    [[GameCenterLauncher shared] clearAllAchivements];
}

#pragma mark -
#pragma mark Leaderboard

void GameCenter::openLeaderboards()
{
    [[GameCenterLauncher shared] openLeaderboard];
}

void GameCenter::postScore(const char* idName, int score)
{
    [[GameCenterLauncher shared] postScore:idName score:[NSNumber numberWithInt:score]];
}

void GameCenter::clearAllScores()
{
    [[GameCenterLauncher shared] clearAllScores];
}