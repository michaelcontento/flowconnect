#include "userstate.h"

#include "Globals.h"
#include "StarButton.h"

#define PREFIX_LEVEL_MODE "lvl"
#define PREFIX_CATEGORY_STARS "cat"
#define KEY_USER_STARS "usr"
#define STARS_STAR_AMOUNT 15

using namespace userstate;
using namespace cocos2d;


void userstate::addStarsToUser(const unsigned int amount)
{
    auto settings = CCUserDefault::sharedUserDefault();
    settings->setIntegerForKey(KEY_USER_STARS, getStarsForUser() + amount);
    settings->flush();

    if (globalLastStarButton) {
        globalLastStarButton->onEnter();
    }
}

int userstate::getStarsForUser()
{
    auto settings = CCUserDefault::sharedUserDefault();
    return settings->getIntegerForKey(KEY_USER_STARS, STARS_STAR_AMOUNT);
}

Mode::Enum userstate::getModeForLevel(const LoaderLevel* level)
{
    auto settings = CCUserDefault::sharedUserDefault();

    auto levelKey = getLevelKey(level);
    auto state = settings->getIntegerForKey(levelKey, Mode::NONE);

    return static_cast<Mode::Enum>(state);
}

void userstate::resetAllLevelModes()
{
    auto settings = CCUserDefault::sharedUserDefault();

    for (auto category : globalLevelLoader.getCategories()) {
        auto categoryKey = getCategoryKey(category);
        if (settings->getIntegerForKey(categoryKey) == 0) {
            continue;
        }
        
        settings->setIntegerForKey(categoryKey, 0);
        for (auto page : category->pages) {
            for (auto level : page->levels) {
                auto levelKey = getLevelKey(level);
                if (settings->getIntegerForKey(levelKey) > 0) {
                    settings->setIntegerForKey(levelKey, Mode::NONE);
                }
            }
        }
    }

    // not required as flush is done in setShowHowToPlay()
    // settings->flush();
    setShowHowToPlay(true);
}

void userstate::setModeForLevel(const LoaderLevel* level, Mode::Enum mode)
{
    auto settings = CCUserDefault::sharedUserDefault();
    
    if (mode == Mode::PERFECT) {
        auto categoryKey = getCategoryKey(level->page->category);
        auto lastValue = getStarsForCategory(level->page->category);
        settings->setIntegerForKey(categoryKey, ++lastValue);

        addStarsToUser(1);
    }
    
    settings->setIntegerForKey(getLevelKey(level), mode);
    settings->flush();
}

int userstate::getStarsForCategory(const LoaderCategory* category)
{
    auto settings = CCUserDefault::sharedUserDefault();

    auto categoryKey = getCategoryKey(category);
    auto result = settings->getIntegerForKey(categoryKey);

    return result;
}

char* userstate::getLevelKey(const LoaderLevel* level)
{
    static char tmpKey[50] = {0};
    snprintf(
        tmpKey, sizeof(tmpKey),
        "%s_%d_%d_%d",
        PREFIX_LEVEL_MODE,
        level->page->category->localid,
        level->page->localid,
        level->localid
    );

    return tmpKey;
}

char* userstate::getCategoryKey(const LoaderCategory* category)
{
    static char tmpKey[50] = {0};
    snprintf(
        tmpKey, sizeof(tmpKey),
        "%s_%d",
        PREFIX_CATEGORY_STARS,
        category->localid
    );

    return tmpKey;
}

bool userstate::showHowToPlay()
{
    auto settings = CCUserDefault::sharedUserDefault();
    return settings->getBoolForKey("show_how_to_play", true);
}

void userstate::setShowHowToPlay(const bool flag)
{
    auto settings = CCUserDefault::sharedUserDefault();
    settings->setBoolForKey("show_how_to_play", flag);
    settings->flush();
}