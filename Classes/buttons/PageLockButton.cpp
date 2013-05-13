#include "PageLockButton.h"

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

    AlertView::createAlert(
        _("alert.unlockpage", "headline").get().c_str(),
        _("alert.unlockpage", "body").assign("amount", PRICE_PAGE).get().c_str(),
        _("alert.unlockpage", "btn.cancel").get().c_str()
    );
    AlertView::addAlertButton(_("alert.unlockpage", "btn.ok").get().c_str());
    AlertView::showAlert(this);

    return true;
}

void PageLockButton::alertViewClickedButtonAtIndex(int buttonIndex)
{
    if (buttonIndex == 0) {
        return;
    }

    if (userstate::addStarsToUser(PRICE_PAGE * -1)) {
        userstate::setIsPageFree(page, true);
        setVisible(false);
    }
}
