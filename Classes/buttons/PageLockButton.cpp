#include "PageLockButton.h"

#include <avalon/ui/Alert.h>
#include <avalon/i18n/LanguageKey.h>
#include <avalon/i18n/Localization.h>
#include "Globals.h"
#include "userstate.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using avalon::i18n::_;

PageLockButton::PageLockButton()
: page(NULL)
{
}


PageLockButton::~PageLockButton()
{
}

PageLockButton* PageLockButton::create(const LoaderPage* page)
{
    auto result = new PageLockButton();
    if (result && result->init(page)) {
        result->autorelease();
        return result;
    } else {
        delete result;
        result = NULL;
        return NULL;
    }
}

bool PageLockButton::init(const LoaderPage* page)
{
    if (!CCSprite::initWithSpriteFrameName("giantlock.png")) {
        return false;
    }

    assert(page && "a page instance is required");
    this->page = page;

    return true;
}

bool PageLockButton::onClick()
{
    if (userstate::isPageFree(page)) {
        return false;
    }

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");

    avalon::ui::Alert alert(this);
    alert.setTitle(_("alert.unlockpage", "headline").get().c_str());
    alert.setMessage(_("alert.unlockpage", "body").assign("amount", PRICE_PAGE).get().c_str());
    alert.addButton(0, _("alert.unlockpage", "btn.cancel").get().c_str());
    alert.addButton(1, _("alert.unlockpage", "btn.ok").get().c_str());
    alert.show();

    return true;
}

void PageLockButton::onAlertButtonClick(const unsigned int index, const std::string title)
{
    if (index == 0) {
        return;
    }

    if (userstate::addStarsToUser(PRICE_PAGE * -1)) {
        userstate::setIsPageFree(page, true);
        setVisible(false);
    }
}
