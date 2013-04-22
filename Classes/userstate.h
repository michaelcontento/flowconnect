#ifndef __FlowConnect__userstate__
#define __FlowConnect__userstate__

#include "cocos2d.h"
#include "LevelLoader.h"

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
