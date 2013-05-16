#ifndef __FlowConnect__PageLockButton__
#define __FlowConnect__PageLockButton__

#include <avalon/ui/AlertDelegate.h>
#include "cocos2d.h"
#include "../LevelLoader.h"

class PageLockButton : public cocos2d::CCSprite, public avalon::ui::AlertDelegate
{
public:
    PageLockButton();
    virtual ~PageLockButton();

    static PageLockButton* create(const LoaderPage* page);
    bool init(const LoaderPage* page);

    bool onClick();
    virtual void onAlertButtonClick(const unsigned int index, const std::string title) override;

private:
    const LoaderPage* page;
};

#endif /* defined(__FlowConnect__PageLockButton__) */
