namespace Cocos2dExt {
    class NativeCodeLauncher
    {
    public:
        // GameCenter
                static void loginGameCenter();
                static void openRanking();
                static void postHighScore(const char* idName,int score);
                static void openAchievement();
                static void postAchievement(const char* idName,int percentComplete);
    };
}
