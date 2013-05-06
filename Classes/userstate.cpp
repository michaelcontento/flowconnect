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

    auto levelKey = getLevelKey(level);
    auto state = settings->getIntegerForKey(levelKey, Mode::NONE);

    return static_cast<Mode::Enum>(state);
}

void userstate::resetAllLevelModes()
{
    auto settings = CCUserDefault::sharedUserDefault();

    for (auto category : globalLevelLoader.getCategories()) {
        auto categoryKey = getCategoryKey(category, false);
        if (settings->getIntegerForKey(categoryKey) == 0) {
            continue;
        }

        // reset mode for every level in the category
        for (auto page : category->pages) {
            for (auto level : page->levels) {
                auto levelKey = getLevelKey(level);
                if (settings->getIntegerForKey(levelKey) > 0) {
                    settings->setIntegerForKey(levelKey, Mode::NONE);
                }
            }
        }

        // reset category star counter (total and perfect)
        settings->setIntegerForKey(categoryKey, 0);
        categoryKey = getCategoryKey(category, true);
        settings->setIntegerForKey(categoryKey, 0);
    }

    for (int catId = 1; catId <= MAX_CATEGORIES; ++catId) {
        for (int pageId = 2; pageId <= PAGES_PER_CATEGORY; ++pageId) {
            settings->setBoolForKey(getPageKey(catId, pageId), false);
        }
    }

    // not required as flush is done in setShowHowToPlay()
    // settings->flush();
    setShowHowToPlay(true);

    // it's important to do this after setShowHowToPlay() because it
    // will change the dirty flag!
    settings->setBoolForKey(KEY_DIRTY, false);
    settings->flush();
}

void userstate::setModeForLevel(const LoaderLevel* level, Mode::Enum mode)
{
    auto gc = Avalon::GameCenter();
    auto settings = CCUserDefault::sharedUserDefault();
    auto lastMode = userstate::getModeForLevel(level);
    char buf[50] = {0};

    if (mode == Mode::PERFECT && lastMode != Mode::PERFECT) {
        auto categoryKey = getCategoryKey(level->page->category, true);
        auto newValue = settings->getIntegerForKey(categoryKey) + 1;
        settings->setIntegerForKey(categoryKey, newValue);
        
        addStarsToUser(1);

        snprintf(buf, sizeof(buf), "com.coragames.dtdng.ac.cat.%d.perfect", level->page->category->localid);
        gc.postAchievement(buf, newValue * 100 / 216);
        for (int i = 1; i <= MAX_RANK_ID; ++i) {
            int percent = newValue * 100 / getRankAmount(i);
            if (percent > 0 && percent <= 100) {
                snprintf(buf, sizeof(buf), "com.coragames.dtdng.ac.rank.%d.perfect", i);
                gc.postAchievement(buf, percent);
            }
        }
    }

    if (lastMode == Mode::NONE && mode != Mode::NONE) {
        auto categoryKey = getCategoryKey(level->page->category, false);
        auto newValue = getStarsForCategory(level->page->category) + 1;
        settings->setIntegerForKey(categoryKey, newValue);

        snprintf(buf, sizeof(buf), "com.coragames.dtdng.ac.cat.%d", level->page->category->localid);
        gc.postAchievement(buf, newValue * 100 / 216);
        for (int i = 1; i <= MAX_RANK_ID; ++i) {
            int percent = newValue * 100 / getRankAmount(i);
            if (percent > 0 && percent <= 100) {
                snprintf(buf, sizeof(buf), "com.coragames.dtdng.ac.rank.%d", i);
                gc.postAchievement(buf, percent);
            }
        }
    }

    settings->setIntegerForKey(getLevelKey(level), mode);
    settings->setBoolForKey(KEY_DIRTY, true);
    settings->flush();
}

int userstate::getStarsForCategory(const LoaderCategory* category)
{
    auto settings = CCUserDefault::sharedUserDefault();

    auto categoryKey = getCategoryKey(category, false);
    auto result = settings->getIntegerForKey(categoryKey);

    return result;
}

char* userstate::getPageKey(const unsigned int categoryId, const unsigned int pageId)
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

char* userstate::getPageKey(const LoaderPage* page)
{
    return getPageKey(page->category->localid, page->localid);
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

char* userstate::getCategoryKey(const LoaderCategory* category, const bool perfect)
{
    static char tmpKey[50] = {0};
    snprintf(
        tmpKey, sizeof(tmpKey),
        "%s_%d_%s",
        PREFIX_CATEGORY_STARS,
        category->localid,
        perfect ? "perfect" : "total"
    );

    return tmpKey;
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