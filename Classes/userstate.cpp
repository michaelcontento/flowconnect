#include "userstate.h"

#include "Globals.h"
#include "StarButton.h"
#include <ctime>
#include "Localization.h"
#include "LanguageKey.h"
#include "AlertView.h"
#include "SceneManager.h"
#include "ShopScene.h"
#include "GameCenter.h"

using namespace userstate;
using namespace cocos2d;

int getRankAmount(const int level)
{
    if (level == 1) {
        return 10;
    } else if (level == 2) {
        return 25;
    } else if (level == 3) {
        return 50;
    } else if (level == 4) {
        return 100;
    } else if (level == 5) {
        return 250;
    } else if (level == 6) {
        return 500;
    } else {
        return 1000;
    }
}

char* getPageKey(const unsigned int categoryId, const unsigned int pageId)
{
    static char tmpKey[50] = {0};
    snprintf(
        tmpKey, sizeof(tmpKey),
        "%s_%d_%d",
        PREFIX_PAGE_FREE,
        categoryId,
        pageId
    );

    return tmpKey;
}

char* getPageKey(const LoaderPage* page)
{
    return getPageKey(page->category->localid, page->localid);
}

char* getLevelKey(const LoaderLevel* level, const char* postfix)
{
    static char tmpKey[50] = {0};
    snprintf(
        tmpKey, sizeof(tmpKey),
        "%s_%d_%d_%d_%s",
        PREFIX_LEVEL_MODE,
        level->page->category->localid,
        level->page->localid,
        level->localid,
        postfix
    );

    return tmpKey;
}

char* getCategoryKey(const LoaderCategory* category, const char* postfix)
{
    static char tmpKey[50] = {0};
    snprintf(
        tmpKey, sizeof(tmpKey),
        "%s_%d_%s",
        PREFIX_CATEGORY_STARS,
        category->localid,
        postfix
    );

    return tmpKey;
}

int getTotalStarsInAllOtherCategories(const LoaderCategory* category, const char* postfix)
{
    int sum = 0;
    auto settings = CCUserDefault::sharedUserDefault();

    for (auto iterCategory : globalLevelLoader.getCategories()) {
        if (iterCategory->localid == category->localid) {
            continue;
        }
        sum += settings->getIntegerForKey(getCategoryKey(iterCategory, postfix));
    }

    return sum;
}

void postAchievement(const char* mode, const LoaderCategory* category, const int value)
{
    auto gc = Avalon::GameCenter();
    char buf[50] = {0};

    snprintf(buf, sizeof(buf), "com.coragames.dtdng.ac.cat.%d", category->localid);
    gc.postAchievement(buf, value * 100 / LEVELS_PER_CATEGORY);

    for (int i = 1; i <= MAX_RANK_ID; ++i) {
        int sum = getTotalStarsInAllOtherCategories(category, mode) + value;
        int percent = sum * 100 / getRankAmount(i);
        
        if (percent == 0 || percent > 100) {
            continue;
        }

        snprintf(
            buf, sizeof(buf),
            "com.coragames.dtdng.ac.rank.%d%s",
            i,
            (strcmp(mode, "perfect") == 0) ? ".perfect" : ""
        );
        gc.postAchievement(buf, percent);
    }
}

class AlertDelegateNotEnoughStars : public AlertViewDelegate
{
public:
    virtual void alertViewClickedButtonAtIndex(int buttonIndex)
    {
        if (buttonIndex == 0) {
            return;
        }
        SceneManager::getInstance().gotoScene(ShopScene::scene());
    }
};


bool userstate::isPageFree(const LoaderPage* page)
{
    if (page->localid == 1) {
        return true;
    }
    
    auto settings = CCUserDefault::sharedUserDefault();
    return settings->getBoolForKey(getPageKey(page));
}

void userstate::setIsPageFree(const LoaderPage* page, const bool flag)
{
    auto settings = CCUserDefault::sharedUserDefault();
    settings->setBoolForKey(getPageKey(page), flag);
    settings->setBoolForKey(KEY_DIRTY, true);
    settings->flush();
}

bool userstate::showAds()
{
    auto settings = CCUserDefault::sharedUserDefault();
    return settings->getBoolForKey(KEY_SHOW_ADS, true);
}

void userstate::setShowAds(const bool flag)
{
    auto settings = CCUserDefault::sharedUserDefault();
    settings->setBoolForKey(KEY_SHOW_ADS, flag);
    settings->flush();
}

bool userstate::resetable()
{
    auto settings = CCUserDefault::sharedUserDefault();
    return settings->getBoolForKey(KEY_DIRTY);
}

int userstate::getFreeHints()
{
    auto settings = CCUserDefault::sharedUserDefault();
    return settings->getIntegerForKey(KEY_HINT_AMOUNT, START_HINTS_AMOUNT);
}

void userstate::addFreeHint(const int amount)
{
    auto settings = CCUserDefault::sharedUserDefault();
    settings->setIntegerForKey(KEY_HINT_AMOUNT, getFreeHints() + amount);
    settings->flush();
}

void userstate::forceRefillFreeHints()
{
    auto settings = CCUserDefault::sharedUserDefault();
    
    auto nowTs = std::time(0);
    settings->setIntegerForKey(KEY_FREE_HINT_COOLDOWN, nowTs);

    auto hintsToAdd = FREE_HINTS_AFTER_COOLDOWN - getFreeHints();
    addFreeHint(hintsToAdd);
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
    snprintf(buf, sizeof(buf), _("dialog.freehints", "body")->getCString(), hintsToAdd);
    CCMessageBox(buf, _("dialog.freehints", "headline")->getCString());
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

bool userstate::addStarsToUser(const unsigned int amount)
{
    static auto alertDelegateNotEnoughStars = new AlertDelegateNotEnoughStars();

    int newAmount = getStarsForUser() + amount;
    if (newAmount < 0) {
        AlertView::createAlert(
            _("alert.notenoughstars", "headline")->getCString(),
            _("alert.notenoughstars", "body")->getCString(),
            _("alert.notenoughstars", "btn.cancel")->getCString()
        );
        AlertView::addAlertButton(_("alert.notenoughstars", "btn.ok")->getCString());
        AlertView::showAlert(alertDelegateNotEnoughStars);
        return false;
    }

    auto settings = CCUserDefault::sharedUserDefault();
    settings->setIntegerForKey(KEY_USER_STARS, newAmount);
    settings->flush();

    if (globalLastStarButton) {
        globalLastStarButton->refreshCounter();
    }

    return true;
}

int userstate::getStarsForUser()
{
    auto settings = CCUserDefault::sharedUserDefault();
    return settings->getIntegerForKey(KEY_USER_STARS, START_STARS_AMOUNT);
}

Mode::Enum userstate::getModeForLevel(const LoaderLevel* level)
{
    auto settings = CCUserDefault::sharedUserDefault();

    auto levelKey = getLevelKey(level, "mode");
    auto state = settings->getIntegerForKey(levelKey, Mode::NONE);

    return static_cast<Mode::Enum>(state);
}

void userstate::resetAllLevelModes()
{
    auto settings = CCUserDefault::sharedUserDefault();
    const char* categoryKey = NULL;
    const char* pageKey = NULL;
    const char* levelKey = NULL;

    for (auto category : globalLevelLoader.getCategories()) {
        if (getStarsForCategory(category) == 0) {
            continue;
        }

        for (auto page : category->pages) {
            if (page->localid > 1 && isPageFree(page)) {
                pageKey = getPageKey(category->localid, page->localid);
                settings->setBoolForKey(pageKey, false);
            }
            
            for (auto level : page->levels) {
                if (getModeForLevel(level) == Mode::NONE) {
                    continue;
                }
                
                levelKey = getLevelKey(level, "mode");
                settings->setIntegerForKey(levelKey, Mode::NONE);
                levelKey = getLevelKey(level, "moves");
                settings->setIntegerForKey(levelKey, 0);
                levelKey = getLevelKey(level, "duration");
                settings->setFloatForKey(levelKey, 0);
            }
        }

        categoryKey = getCategoryKey(category, "total");
        settings->setIntegerForKey(categoryKey, 0);
        categoryKey = getCategoryKey(category, "perfect");
        settings->setIntegerForKey(categoryKey, 0);
        categoryKey = getCategoryKey(category, "duration");
        settings->setFloatForKey(categoryKey, 0);
    }

    setShowHowToPlay(true);
    
    // it's important to do this after setShowHowToPlay() because it
    // will change the dirty flag!
    settings->setBoolForKey(KEY_DIRTY, false);
    settings->flush();
}

void userstate::setModeForLevel(const LoaderLevel* level, Mode::Enum mode)
{
    auto settings = CCUserDefault::sharedUserDefault();
    auto lastMode = userstate::getModeForLevel(level);

    if (mode == Mode::PERFECT && lastMode != Mode::PERFECT) {
        auto categoryKey = getCategoryKey(level->page->category, "perfect");
        auto newValue = settings->getIntegerForKey(categoryKey) + 1;
        settings->setIntegerForKey(categoryKey, newValue);
        
        addStarsToUser(1);
        postAchievement("perfect", level->page->category, newValue);
    }

    if (lastMode == Mode::NONE && mode != Mode::NONE) {
        auto categoryKey = getCategoryKey(level->page->category, "total");
        auto newValue = getStarsForCategory(level->page->category) + 1;
        settings->setIntegerForKey(categoryKey, newValue);

        postAchievement("total", level->page->category, newValue);
    }

    settings->setIntegerForKey(getLevelKey(level, "mode"), mode);
    settings->setBoolForKey(KEY_DIRTY, true);
    settings->flush();
}

int userstate::getStarsForCategory(const LoaderCategory* category)
{
    auto settings = CCUserDefault::sharedUserDefault();

    auto categoryKey = getCategoryKey(category, "total");
    auto result = settings->getIntegerForKey(categoryKey);

    return result;
}

bool userstate::showHowToPlay()
{
    auto settings = CCUserDefault::sharedUserDefault();
    return settings->getBoolForKey(KEY_SHOW_HOWTO, true);
}

void userstate::setShowHowToPlay(const bool flag)
{
    auto settings = CCUserDefault::sharedUserDefault();
    settings->setBoolForKey(KEY_SHOW_HOWTO, flag);
    settings->setBoolForKey(KEY_DIRTY, true);
    settings->flush();
}

bool userstate::isNumberMode()
{
    auto settings = CCUserDefault::sharedUserDefault();
    return settings->getBoolForKey(KEY_NUMBER_MODE, true);
}

void userstate::setIsNumberMode(const bool flag)
{
    auto settings = CCUserDefault::sharedUserDefault();
    settings->setBoolForKey(KEY_NUMBER_MODE, flag);
    settings->flush();
}

void userstate::updateLevelDuration(const LoaderLevel* level, const float duration)
{
    auto settings = CCUserDefault::sharedUserDefault();
    auto gc = Avalon::GameCenter();

    auto lastKey = getLevelKey(level, "duration");
    auto last = settings->getFloatForKey(lastKey);

    auto lastSumKey = getCategoryKey(level->page->category, "duration");
    auto lastSum = settings->getFloatForKey(lastSumKey);
    auto newSum = lastSum + duration - last;
    
    if (last == 0 || duration < last) {
        settings->setFloatForKey(lastKey, duration);
        settings->setFloatForKey(lastSumKey, newSum);
        settings->flush();
    }

    char buf[50] = {0};
    snprintf(buf, sizeof(buf), "com.coragames.dtdng.lb.%d", level->page->category->localid);
    
    int handicap = LEVELS_PER_CATEGORY - getStarsForCategory(level->page->category);
    // value = (HOURS * 360000) + (MINUTES * 6000) + (SECONDS * 100) + CENTISECONDS;
    gc.postScore(buf, (handicap * 360000) + (newSum * 100));
}

float userstate::getLevelDuration(const LoaderLevel* level)
{
    auto settings = CCUserDefault::sharedUserDefault();
    return settings->getFloatForKey(getLevelKey(level, "duration"));
}

void userstate::updateLevelMoves(const LoaderLevel* level, const int moves)
{
    auto settings = CCUserDefault::sharedUserDefault();
    auto key = getLevelKey(level, "moves");
    auto last = settings->getIntegerForKey(key);

    if (last == 0 || moves < last) {
        settings->setIntegerForKey(key, moves);
        settings->flush();
    }
}

int userstate::getLevelMoves(const LoaderLevel* level)
{
    auto settings = CCUserDefault::sharedUserDefault();
    return settings->getIntegerForKey(getLevelKey(level, "moves"));
}