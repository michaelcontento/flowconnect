#include "userstate.h"

#include "Globals.h"
#include "StarButton.h"
#include <ctime>
#include "Localization.h"
#include "LanguageKey.h"
#include "AlertView.h"
#include "SceneManager.h"
#include "ShopScene.h"

using namespace userstate;
using namespace cocos2d;

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

    // it's important to do this after setShowHowToPlay() because it
    // will change the dirty flag!
    settings->setBoolForKey(KEY_DIRTY, false);
    settings->flush();
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
    settings->setBoolForKey(KEY_DIRTY, true);
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