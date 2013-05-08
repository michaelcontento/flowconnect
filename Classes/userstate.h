#ifndef __FlowConnect__userstate__
#define __FlowConnect__userstate__

#include "cocos2d.h"
#include "LevelLoader.h"

#define PREFIX_LEVEL_MODE "lvl"
#define PREFIX_CATEGORY_STARS "cat"
#define PREFIX_PAGE_FREE "page_free"

#define MAX_RANK_ID 7
#define LEVELS_PER_CATEGORY 216

#define KEY_USER_STARS "stars"
#define START_STARS_AMOUNT 0

#define FREE_STARS 10

#define KEY_SHOW_HINT_WARNING "hint_warning"
#define KEY_HINT_AMOUNT "hints"
#define START_HINTS_AMOUNT 5

#define KEY_FREE_HINT_COOLDOWN "hint_cooldown"
#define FREE_HINTS_AFTER_COOLDOWN START_HINTS_AMOUNT
#define FREE_HINTS_COOLDOWN_IN_SEC (24 * 60 * 60)

#define KEY_DIRTY "dirty"
#define KEY_SHOW_HOWTO "show_how_to_play"

#define KEY_SHOW_ADS "show_ads"
#define KEY_NUMBER_MODE "number_mode"

namespace userstate
{
    namespace Mode
    {
        enum Enum {
            NONE = 0,
            SOLVED,
            PERFECT
        };
    }

    bool isPageFree(const LoaderPage* page);
    void setIsPageFree(const LoaderPage* page, const bool flag);

    bool showAds();
    void setShowAds(const bool flag);

    void enableMusic(const bool flag = true);
    bool hasMusic();

    void enableSounds(const bool flag = true);
    bool hasSounds();

    bool isNumberMode();
    void setIsNumberMode(const bool flag);

    bool resetable();
    void forceRefillFreeHints();

    bool fbLikeDone();
    void fbLike();

    bool rateUsDone();
    void rateUs();

    int getFreeHints();
    void addFreeHint(const int amount);
    void refreshFreeHints();

    bool showHintWarning();
    void setHintWarning(const bool flag);

    bool showHowToPlay();
    void setShowHowToPlay(const bool flag);

    bool addStarsToUser(const unsigned int amount);
    void resetAllLevelModes();

    int getStarsForUser();
    int getStarsForCategory(const LoaderCategory* category);

    Mode::Enum getModeForLevel(const LoaderLevel* level);
    void setModeForLevel(const LoaderLevel* level, Mode::Enum mode);

    void updateLevelDuration(const LoaderLevel* level, const float duration);
    float getLevelDuration(const LoaderLevel* level);

    void updateLevelMoves(const LoaderLevel* level, const int moves);
    int getLevelMoves(const LoaderLevel* level);
}


#endif /* defined(__FlowConnect__userstate__) */
