#import "GameCenter.h"
#import "GameCenterLauncher.h"

void GameCenter::login()
{
    [GameCenterLauncher login];
}

void GameCenter::openAchievement()
{
    [GameCenterLauncher openAchievement];
}

void GameCenter::postAchievement(const char* idName, int percentComplete)
{
    [GameCenterLauncher postAchievement:idName percent:[NSNumber numberWithInt:percentComplete]];
}

void GameCenter::openLeaderboards()
{
    [GameCenterLauncher openLeaderboard];
}

void GameCenter::postScore(const char* idName, int score)
{
    [GameCenterLauncher postScore:idName score:[NSNumber numberWithInt:score]];
}