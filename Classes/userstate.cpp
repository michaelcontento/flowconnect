#include "userstate.h"

#define PREFIX_LEVEL_MODE "level_mode"
#define PREFIX_CATEGORY_STARS "category_stars"
#define KEY_USER_STARS "user_stars"
#define STARS_STAR_AMOUNT 15

using namespace userstate;
using namespace cocos2d;


void userstate::addStarsToUser(const unsigned int amount)
{
    auto settings = CCUserDefault::sharedUserDefault();
    settings->setIntegerForKey(KEY_USER_STARS, getStarsForUser() + amount);
    settings->flush();
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
    free(levelKey);

    return static_cast<Mode::Enum>(state);
}

void userstate::setModeForLevel(const LoaderLevel* level, Mode::Enum mode)
{
    auto settings = CCUserDefault::sharedUserDefault();
    
    auto levelKey = getLevelKey(level);
    settings->setIntegerForKey(levelKey, mode);
    free(levelKey);

    auto lastValue = getStarsForCategory(level->page->category);
    auto categoryKey = getCategoryKey(level->page->category);
    settings->setIntegerForKey(categoryKey, ++lastValue);
    free(categoryKey);

    // not required as flush is done in addStarsToUser()
    // settings->flush();
    addStarsToUser(1);
}

int userstate::getStarsForCategory(const LoaderCategory* category)
{
    auto settings = CCUserDefault::sharedUserDefault();

    auto categoryKey = getCategoryKey(category);
    auto result = settings->getIntegerForKey(categoryKey, 0);
    free(categoryKey);

    return result;
}

char* userstate::getLevelKey(const LoaderLevel* level)
{
    std::stringstream key;
    key << PREFIX_LEVEL_MODE;
    key << "_";
    key << level->page->category->localid;
    key << "_";
    key << level->page->localid;
    key << "_";
    key << level->localid;

    return strdup(key.str().c_str());
}

char* userstate::getCategoryKey(const LoaderCategory* category)
{
    std::stringstream key;
    key << PREFIX_CATEGORY_STARS;
    key << "_";
    key << category->localid;

    return strdup(key.str().c_str());
}