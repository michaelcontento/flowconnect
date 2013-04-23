#include "userstate.h"

#include "Globals.h"
#include "StarButton.h"
#include <ctime>

using namespace userstate;
using namespace cocos2d;

int userstate::getFreeHints()
{
    auto settings = CCUserDefault::sharedUserDefault();
    return settings->getIntegerForKey(KEY_HINT_AMOUNT, START_HINTS_AMOUNT);
}

void userstate::addFreeHint(const int amount)
{
    auto settings = CCUserDefault::sharedUserDefault();
    return settings->setIntegerForKey(KEY_HINT_AMOUNT, getFreeHints() + amount);
    settings->flush();
}

void userstate::refreshFreeHints()
{
    auto settings = CCUserDefault::sharedUserDefault();
    auto lastRefresh = settings->getIntegerForKey(KEY_FREE_HINT_COOLDOWN, -1);
    auto nowTs = std::time(0);

    // first app start should NOT add additional free hints.
    // see START_HINTS_AMOUNT for this.
    if (lastRefresh == -1) {
        settings->setIntegerForKey(KEY_FREE_HINT_COOLDOWN, nowTs);
        settings->flush();
        return;
    }

    // cooldown still active
    if (lastRefresh + FREE_HINTS_COOLDOWN_IN_SEC > nowTs) {
        return;
    }

    // it's important to set and flush this here, as it could be that
    // the user already has enough free hints. but with this we disable this
    // check for the next FREE_HINTS_COOLDOWN_IN_SEC
    settings->setIntegerForKey(KEY_FREE_HINT_COOLDOWN, nowTs);
    settings->flush();

    // yup .. user is full of free hints
    auto hintsToAdd = FREE_HINTS_AFTER_COOLDOWN - getFreeHints();
    if (hintsToAdd == 0) {
        return;
    }

    addFreeHint(hintsToAdd);
    setHintWarning(true);

    char buf[100] = {0};
    snprintf(buf, sizeof(buf), "du hast %d free hints bekommen!", hintsToAdd);
    CCMessageBox(buf, "welcome back");
}

bool userstate::showHintWarning()
{
    auto settings = CCUserDefault::sharedUserDefault();
    return settings->getBoolForKey(KEY_SHOW_HINT_WARNING, true);
}

void userstate::setHintWarning(const bool flag)
{
    auto settings = CCUserDefault::sharedUserDefault();
    settings->setBoolForKey(KEY_SHOW_HINT_WARNING, flag);
    settings->flush();
}

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
    return settings->getIntegerForKey(KEY_USER_STARS, START_STARS_AMOUNT);
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