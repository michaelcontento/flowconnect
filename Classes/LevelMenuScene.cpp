#include "LevelMenuScene.h"

#include "ButtonFactory.h"
#include "Colors.h"

using namespace cocos2d;
using namespace cocos2d::extension;

extern LoaderLevel* globalLevel;

#pragma mark Initialization

LevelMenuScene::LevelMenuScene()
: scrollView(NULL)
, pageIndicator(NULL)
, totalPages(0)
{
}

LevelMenuScene::~LevelMenuScene()
{
    CC_SAFE_RELEASE_NULL(pageIndicator);
}

CCScene* LevelMenuScene::scene()
{
    CCScene *scene = CCScene::create();
    scene->addChild(LevelMenuScene::create());
    return scene;
}

bool LevelMenuScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    CCDirector::sharedDirector()
        ->getTouchDispatcher()
        ->addTargetedDelegate(this, 1, false);

    scrollView = CCScrollView::create(CCSize(768, 1024));
    scrollView->setDirection(kCCScrollViewDirectionHorizontal);
    scrollView->setPosition(CCPointZero);
    scrollView->setContentOffset(CCPointZero);
    scrollView->setContainer(createMenuContainer());
    scrollView->setClippingToBounds(true);
    addChild(scrollView);

    auto indicator = createPageIndicatorContainer();
    indicator->setAnchorPoint(CCPoint(0.5, 0.5));
    indicator->setPosition(CCPoint(384, 120));
    addChild(indicator);

    auto backButton = ButtonFactory::createSceneBackButton();
    addChild(backButton);

    auto headline = CCLabelTTF::create(globalLevel->page->category->name, "Markler Fett", 56);
    headline->setAnchorPoint(CCPoint(0.5, 0.5));
    headline->setPositionX(768 / 2);
    headline->setPositionY(backButton->getPositionY());
    addChild(headline);

    return true;
}

CCNode* LevelMenuScene::createPageIndicatorContainer()
{
    assert(!pageIndicator && "pageIndicator array already set");
    assert(scrollView && "scrollView is required to be set");

    pageIndicator = CCArray::create();
    pageIndicator->retain();
    auto container = CCNode::create();
    auto size = CCSize();

    for (int page = 0; page < totalPages; ++page) {
        auto indicator = CCSprite::createWithSpriteFrameName("buttons/pageindicator.png");
        indicator->setAnchorPoint(CCPointZero);
        indicator->setPositionX(size.width);

        size.height = MAX(size.height, indicator->getContentSize().height);
        size.width += indicator->getContentSize().width + INDICATOR_SPACING;

        pageIndicator->addObject(indicator);
        container->addChild(indicator);
    }

    size.width -= INDICATOR_SPACING;
    container->setContentSize(size);

    setPageIndicatorPage(0);

    return container;
}

void LevelMenuScene::setPageIndicatorPage(const unsigned int page)
{
    CCObject* it = NULL;
    int iterPage = -1;

    CCARRAY_FOREACH(pageIndicator, it) {
        auto indicator = static_cast<CCSprite*>(it);
        ++iterPage;

        if (page == iterPage) {
            indicator->setOpacity(255 * 0.9);
        } else {
            indicator->setOpacity(255 * 0.4);
        }
    }
}

void LevelMenuScene::adjustScrollView()
{
    assert(scrollView && "scrollView is required to be set");

    auto offset = scrollView->getContentOffset();
    int pageOffset = (int)offset.x % 768;
    auto page = abs(offset.x) / 768;

    if (pageOffset <= -384) {
        ++page;
    }

    auto maxOffset = (scrollView->getContentSize().width - 768) * -1;
    auto newOffset = CCPoint(page * 768 * -1, offset.y);
    newOffset.x = fmax(newOffset.x, maxOffset);

    setPageIndicatorPage(MIN(page, totalPages - 1));
    
    scrollView->unscheduleAllSelectors();
    scrollView->setContentOffset(newOffset, true);
}

CCNode* LevelMenuScene::createMenuContainer()
{
    CCNode* container = CCNode::create();
    
    // add all pages
    totalPages = 0;
    for (auto page : globalLevel->page->category->pages) {
        auto pageMenu  = createPageMenu(page);
        pageMenu->setPosition(768 * totalPages++, 0);
        
        container->addChild(pageMenu);
    }

    // detect the right content size
    auto firstObject = container->getChildren()->objectAtIndex(0);
    auto firstPage = dynamic_cast<CCNode*>(firstObject);
    container->setContentSize(CCSize(
        totalPages * 768,
        firstPage->getContentSize().height
    ));

    // positioning
    container->setPositionX(
        container->getPositionX()
        + (768 - firstPage->getContentSize().width) / 2
    );
    container->setPositionY(
        container->getPositionY()
        + (1024 - firstPage->getContentSize().height) / 2
    );

    // we need to wrap the container again or the positioning
    // we've done before would be removed by CCScrollView :/
    auto posContainer = CCNode::create();
    posContainer->setContentSize(container->getContentSize());
    posContainer->addChild(container);

    return posContainer;
}

CCNode* LevelMenuScene::createPageMenu(const LoaderPage* page) const
{
    auto menu = CCMenu::create();
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);

    for (auto level : page->levels) {
        auto button = ButtonFactory::createLevelButton(level);
        button->setBorderColor(LINE_COLORS[page->localid]);
        menu->addChild(button);
    }

    alignMenu(menu);

    auto container = CCNode::create();
    container->setAnchorPoint(CCPointZero);
    container->setPosition(CCPointZero);
    container->addChild(menu);
    container->setContentSize(menu->getContentSize());

    auto headline = CCLabelTTF::create(page->name, "Markler Fett", 24);
    headline->setAnchorPoint(CCPoint(0, 0));
    headline->setPosition(CCPoint(0, menu->getContentSize().height));
    container->addChild(headline);

    return container;
}

void LevelMenuScene::alignMenu(CCMenu* menu) const
{
    auto btnPos = CCPoint();
    auto btnSize = CCSize();
    auto pageSize = CCSize();

    CCObject* it = NULL;
    CCARRAY_FOREACH_REVERSE(menu->getChildren(), it) {
        auto button = dynamic_cast<GameButton*>(it);

        btnSize.width = (button->getContentSize().width + ITEM_PADDING);
        btnSize.height = (button->getContentSize().height + ITEM_PADDING);

        if (btnPos.x == 0) {
            pageSize.height += btnSize.height;
        }
        if (btnPos.y == 0) {
            pageSize.width += btnSize.width;
        }

        button->setAnchorPoint(CCPointZero);
        button->setPosition(CCPoint(
            btnSize.width * (ITEMS_PER_ROW - ++btnPos.x),
            btnSize.height * btnPos.y
        ));

        if (btnPos.x >= ITEMS_PER_ROW) {
            btnPos.x = 0;
            ++btnPos.y;
        }
    }

    pageSize.width -= ITEM_PADDING;
    pageSize.height -= ITEM_PADDING;
    menu->setContentSize(pageSize);
}

bool LevelMenuScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void LevelMenuScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
}

void LevelMenuScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    adjustScrollView();
}

void LevelMenuScene::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    adjustScrollView();
}
