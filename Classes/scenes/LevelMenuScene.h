#ifndef __FlowConnect__LevelMenuScene__
#define __FlowConnect__LevelMenuScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <map>

class LoaderPage;
class PageLockButton;

class LevelMenuScene : public cocos2d::CCLayer
{
public:
    static const LoaderPage* scrollTo;
    
    LevelMenuScene();
    virtual ~LevelMenuScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(LevelMenuScene);
    virtual bool init() override;

    void btnGame(void* sender) const;
    
    virtual void keyBackClicked() override;
    virtual void onEnter() override;
    virtual void onExit() override;

    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) override;
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) override;
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) override;
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) override;

private:
    static const unsigned int ITEMS_PER_ROW = 6;
    static const unsigned int ITEM_PADDING = 15;
    static const unsigned int INDICATOR_SPACING = 10;

    unsigned short int touchCounter;
    bool isFirstEnter;
    cocos2d::CCArray* pageIndicator;
    cocos2d::CCArray* buttons;
    unsigned int totalPages;
    int currentPage;
    cocos2d::extension::CCScrollViewWithMenu* scrollView;
    mutable std::map<const LoaderPage*, PageLockButton*> pageLockButtons;

    cocos2d::CCNode* createMenuContainer();
    cocos2d::CCNode* createPageMenu(const LoaderPage* page) const;
    void alignMenu(cocos2d::CCMenu* menu) const;
    void adjustScrollView(const LoaderPage* = NULL);

    cocos2d::CCNode* createPageIndicatorContainer();
    void setPageIndicatorPage();
};

#endif /* defined(__FlowConnect__LevelMenuScene__) */
