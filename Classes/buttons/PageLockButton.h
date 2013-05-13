#ifndef __FlowConnect__PageLockButton__
#define __FlowConnect__PageLockButton__

#include "cocos2d.h"
#include "LevelLoader.h"
#include "AlertView.h"

class PageLockButton : public cocos2d::CCSprite, public AlertViewDelegate
{
public:
    PageLockButton();
    virtual ~PageLockButton();

    static PageLockButton* create(const LoaderPage* page);
    bool init(const LoaderPage* page);

    bool onClick();
    virtual void alertViewClickedButtonAtIndex(int buttonIndex);

private:
    const LoaderPage* page;
};

#endif /* defined(__FlowConnect__PageLockButton__) */
