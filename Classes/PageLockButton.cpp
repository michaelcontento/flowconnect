#include "PageLockButton.h"

#include "Globals.h"
#include "userstate.h"
#include "Localization.h"
#include "LanguageKey.h"

using namespace cocos2d;

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

    static char buf[250] = {0};
    snprintf(buf, sizeof(buf), _("alert.unlockpage", "body")->getCString(), PRICE_PAGE);

    AlertView::createAlert(
        _("alert.unlockpage", "headline")->getCString(),
        buf,
        _("alert.unlockpage", "btn.cancel")->getCString()
    );
    AlertView::addAlertButton(_("alert.unlockpage", "btn.ok")->getCString());
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
