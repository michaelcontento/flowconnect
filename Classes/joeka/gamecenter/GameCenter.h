#ifndef __AVALON_GAMECENTER__
#define __AVALON_GAMECENTER__

class GameCenter
{
public:
    void login();

    void openAchievement();
    void postAchievement(const char* idName, int percentComplete);

    void openLeaderboards();
    void postScore(const char* idName, int score);
};

#endif