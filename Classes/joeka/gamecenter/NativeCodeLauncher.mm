#import "NativeCodeLauncher.h"
#import "NativeCodeLauncher_objc.h"

// GameCenterにログイン
static void static_loginGameCenter()
{
        [NativeCodeLauncher loginGameCenter];
}
// Leaderboard表示
static void static_openRanking()
{
        [NativeCodeLauncher openRanking];
}
// ハイスコアを送信
static void static_postHighScore(const char* idName,int score)
{
        [NativeCodeLauncher postHighScore:idName score:[NSNumber numberWithInt:score]];
}
// Achievement表示
static void static_openAchievement()
{
        [NativeCodeLauncher openAchievement];
}
// 達成項目を送信
static void static_postAchievement(const char* idName,int percentComplete)
{
        [NativeCodeLauncher postAchievement:idName percent:[NSNumber numberWithInt:percentComplete]];
}

namespace Cocos2dExt
{
    // GameCenter
        void NativeCodeLauncher::loginGameCenter()
        {
                static_loginGameCenter();
        }
        void NativeCodeLauncher::openRanking()
        {
                static_openRanking();
        }
        void NativeCodeLauncher::postHighScore(const char* idName, int score)
        {
                static_postHighScore(idName, score);
        }
        void NativeCodeLauncher::openAchievement()
        {
                static_openAchievement();
        }
        void NativeCodeLauncher::postAchievement(const char* idName, int percentComplete)
        {
                static_postAchievement(idName, percentComplete);
        }
}
