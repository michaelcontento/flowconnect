#ifndef __FlowConnect__LevelMenuScene__
#define __FlowConnect__LevelMenuScene__

#include "cocos2d.h"
#include "cocos-ext.h"

class LoaderPage;

class LevelMenuScene : public cocos2d::CCLayer
{
public:
    LevelMenuScene();
    virtual ~LevelMenuScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(LevelMenuScene);
    virtual bool init();

    void btnGame(void* sender) const;
    
    virtual void onEnter();

    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

private:
    const unsigned int ITEMS_PER_ROW = 6;
    const unsigned int ITEM_PADDING = 15;
    const unsigned int INDICATOR_SPACING = 10;

    bool isFirstEnter;
    cocos2d::CCArray* pageIndicator;
    cocos2d::CCArray* buttons;
    unsigned int totalPages;
    int currentPage;
    cocos2d::extension::CCScrollViewWithMenu* scrollView;

    cocos2d::CCNode* createMenuContainer();
    cocos2d::CCNode* createPageMenu(const LoaderPage* page) const;
    void alignMenu(cocos2d::CCMenu* menu) const;
    void adjustScrollView();

    cocos2d::CCNode* createPageIndicatorContainer();
    void setPageIndicatorPage();
};

#endif /* defined(__FlowConnect__LevelMenuScene__) */
