#ifndef __FlowConnect__userstate__
#define __FlowConnect__userstate__

#include "cocos2d.h"
#include "LevelLoader.h"

#define PREFIX_LEVEL_MODE "lvl"
#define PREFIX_CATEGORY_STARS "cat"

#define KEY_USER_STARS "stars"
#define START_STARS_AMOUNT 0

#define KEY_SHOW_HINT_WARNING "hint_warning"
#define KEY_HINT_AMOUNT "hints"
#define START_HINTS_AMOUNT 5

#define KEY_FREE_HINT_COOLDOWN "hint_cooldown"
#define FREE_HINTS_AFTER_COOLDOWN START_HINTS_AMOUNT
#define FREE_HINTS_COOLDOWN_IN_SEC (24 * 60 * 60)

#define KEY_DIRTY "dirty"
#define KEY_SHOW_HOWTO "show_how_to_play"

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

    bool resetable();
    void forceRefillFreeHints();

    int getFreeHints();
    void addFreeHint(const int amount);
    void refreshFreeHints();

    bool showHintWarning();
    void setHintWarning(const bool flag);

    bool showHowToPlay();
    void setShowHowToPlay(const bool flag);

    void addStarsToUser(const unsigned int amount);
    void resetAllLevelModes();

    int getStarsForUser();
    int getStarsForCategory(const LoaderCategory* category);

    Mode::Enum getModeForLevel(const LoaderLevel* level);
    void setModeForLevel(const LoaderLevel* level, Mode::Enum mode);

    char* getLevelKey(const LoaderLevel* level);
    char* getCategoryKey(const LoaderCategory* category);
}


#endif /* defined(__FlowConnect__userstate__) */
