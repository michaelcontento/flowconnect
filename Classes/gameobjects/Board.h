#ifndef __FlowConnect__Board__
#define __FlowConnect__Board__

#include "cocos2d.h"
#include "Slot.h"
#include "../LevelLoader.h"

#define BOARD_ZORDER_TOUCH_INDICATOR 10

class GameScene;

class Board : public cocos2d::CCLayer
{
public:
    Board();
    virtual ~Board();

    CREATE_FUNC(Board);
    virtual bool init() override;
    bool initWithLevel(const LoaderLevel* level, GameScene* gameScene);

    CC_SYNTHESIZE_READONLY(cocos2d::CCSize, size, Size);
    CC_SYNTHESIZE_READONLY(const LoaderLevel*, level, Level);

    CC_SYNTHESIZE_READONLY(float, progress, Progress);
    CC_SYNTHESIZE_READONLY(int, moves, Moves);
    CC_SYNTHESIZE_READONLY(float, duration, Duration);
    bool isFinished() const;

    void updateDuration(float dt);
    void reset();
    bool finishTillNextCheckpoint();

    virtual void onEnter() override;
    virtual void onExit() override;

    CC_SYNTHESIZE(float, touchIndicatorScale, TouchIndicatorScale);

    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) override;
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) override;
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) override;
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) override;

private:
    cocos2d::CCArray* slots;
    cocos2d::CCArray* userPath;
    std::vector<char> directions;
    GameScene* gameScene;
    Slot* lastCheckpoint;
    cocos2d::CCSprite* touchIndicator;
    bool allCheckpointVisited;
    bool timerStarted;
    int numFreeSlots;
    bool finishReported;

    void removeAllSlots();
    void createSlotsFromData(const char* data);
    void positionSlotsOnScreen();
    void clearAllSlotsAfter(Slot* slot) const;
    Slot* activateNextCheckpoint();
    void lockCompleteLines() const;
    Slot* getLastUserPathSlot() const;
    Slot* getUserPathSlotBefore(const Slot* slot) const;
    void appendUserPath(Slot* slot);
    void handleAllCheckpointsVisited();
    void startGameTimer();

    void startTouchIndicatorBlink();
    void stopTouchIndicatorBlink();
    bool isFirstCheckpoint(const Slot* slot) const;
    void createTouchIndicator();
    int convert2dTo1dIndex(const cocos2d::CCPoint grid) const;
    cocos2d::CCPoint get2dIndexFromPoint(const cocos2d::CCPoint point) const;
    Slot* getSlotFromPoint(const cocos2d::CCPoint point) const;
};

#endif /* defined(__FlowConnect__Board__) */