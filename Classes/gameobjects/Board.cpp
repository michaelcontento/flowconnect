#include "Board.h"

#include "Token.h"

using namespace cocos2d;

#pragma mark Initialization

Board::Board()
: directions()
, slots(NULL)
, userPath(NULL)
, lastCheckpoint(NULL)
, touchIndicator(NULL)
, allCheckpointVisited(false)
, numFreeSlots(0)
, moves(0)
{
}

Board::~Board()
{
    removeAllSlots();
    CC_SAFE_RELEASE_NULL(slots);

    CC_SAFE_RELEASE_NULL(userPath);
}

bool Board::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    
    slots = CCArray::create();
    CC_SAFE_RETAIN(slots);

    userPath = CCArray::create();
    CC_SAFE_RETAIN(userPath);
    
    CCDirector::sharedDirector()
        ->getTouchDispatcher()
        ->addTargetedDelegate(this, 0, true);

    return true;
}

bool Board::initWithLevel(const cocos2d::CCSize newSize, const char* data)
{
    size = newSize;
    numFreeSlots = size.width * size.height;

    removeAllSlots();
    createSlotsFromData(data);
    
    positionSlotsOnScreen();
    slots->reduceMemoryFootprint();

    return true;
}

#pragma mark Touch handling

bool Board::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    const CCPoint touchPos = convertTouchToNodeSpace(pTouch);
    Slot* slot = getSlotFromPoint(touchPos);

    if (!slot){
        return false;
    }

    if (slot->isCheckpoint()) {
        if (isFirstCheckpoint(slot) || !slot->isFree()) {
            lastCheckpoint = slot;
            slot->setLineOut(SlotLineType::NONE);
        } else {
            return false;
        }
    } else {
        if (!slot->isFree()) {
            slot->setLineOut(SlotLineType::NONE);
        } else {
            return false;
        }
    }

    int slotDiff = userPath->count();

    clearAllSlotsAfter(slot);
    appendUserPath(slot);

    slotDiff -= userPath->count();
    if (slot->getNumber() == 1) {
        moves = 0;
    } else {
        moves += slotDiff;
    }

    createTouchIndicator();
    touchIndicator->setPosition(touchPos);

    return true;
}

void Board::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    // no checkpoint left to visit? the user might a) proceed to the next level
    // or b) restart somewhere with _a new_ touch.
    if (allCheckpointVisited) {
        return;
    }

    const CCPoint touchPos = convertTouchToNodeSpace(pTouch);
    Slot* currentSlot = getSlotFromPoint(touchPos);

    if (!currentSlot) {
        return;
    }

    Slot* lastSlot = getLastUserPathSlot();
    assert(lastSlot && "should always be there because of ccTouchBegin");

    // ----- Validate movement

    CCPoint lastGridIdx = lastSlot->gridIndex;
    CCPoint currentGridIdx = get2dIndexFromPoint(touchPos);

    int distanceX = lastGridIdx.x - currentGridIdx.x;
    int distanceY = lastGridIdx.y - currentGridIdx.y;
    int distanceSum = abs(distanceX) + abs(distanceY);

    if (distanceSum == 0) {
        touchIndicator->setPosition(touchPos);
        return;
    }

    if (distanceSum > 1) {
        return;
    }

    if (currentSlot->isCheckpoint()) {
        if (currentSlot->isNextSlot()) {
            // allowed because it's the next checkpoint
        } else if (currentSlot == getUserPathSlotBefore(lastSlot)) {
            // allowed because we're walking backwards
        } else {
            return;
        }
    } else {
        if (currentSlot->isLocked() && !lastSlot->isLocked()) {
            return;
        }
    }

    // ----- Update slots

    ++moves;

    if (!lastSlot->isFree() && !currentSlot->isFree()) {
        currentSlot->setLineOut(SlotLineType::NONE);
        lastSlot->reset();
        clearAllSlotsAfter(currentSlot);
    } else if (distanceX == -1) {
        lastSlot->setLineOut(SlotLineType::RIGHT);
        currentSlot->setLineIn(SlotLineType::LEFT);
    } else if (distanceX == 1 ) {
        lastSlot->setLineOut(SlotLineType::LEFT);
        currentSlot->setLineIn(SlotLineType::RIGHT);
    } else if (distanceY == -1) {
        lastSlot->setLineOut(SlotLineType::BOTTOM);
        currentSlot->setLineIn(SlotLineType::TOP);
    } else if (distanceY == 1) {
        lastSlot->setLineOut(SlotLineType::TOP);
        currentSlot->setLineIn(SlotLineType::BOTTOM);
    } else {
        assert(false && "this shouldn't happen at all");
    }

    touchIndicator->setPosition(touchPos);
    appendUserPath(currentSlot);
}

void Board::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
}

void Board::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}

#pragma mark Slot handling

bool Board::isFinished() const
{
    return (allCheckpointVisited && numFreeSlots == 0);
}

void Board::appendUserPath(Slot* slot)
{
    Slot* lastSlot = getLastUserPathSlot();
    if (lastSlot != slot) {
        userPath->addObject(slot);
    }
    
    activateNextCheckpoint();
    lockCompleteLines();
}

void Board::removeAllSlots()
{
    CCObject* it;
    CCARRAY_FOREACH(slots, it) {
        Slot* slot = static_cast<Slot*>(it);

        if (slot) {
            removeChild(slot);
        }
    }

    slots->removeAllObjects();
    directions.clear();
}

void Board::createSlotsFromData(const char* data)
{
    char currentToken[2] = {0};
    int len = strlen(data);
    int nextNumber = 0;

    for (int i = 0; i < len; ++i) {
        currentToken[0] = data[i];

        if (!token::isDirectionToken(currentToken)) {
            assert(nextNumber == 0 && "two following numbers are invalid");
            nextNumber = atoi(currentToken);
            continue;
        }

        Slot* nextSlot = Slot::create();
        slots->addObject(nextSlot);
        
        if (nextNumber > 0) {
            nextSlot->setNumber(nextNumber);
            if (nextNumber == 1) {
                nextSlot->markAsNextSlot(true);
            }
            nextNumber = 0;
        }

        addChild(nextSlot);
        directions.push_back(token::convertTokenToSlotDirection(currentToken));
    }

    assert(nextNumber == 0 && "all numbers must be assigned");
}

void Board::positionSlotsOnScreen()
{
    assert(slots->count() > 0 && "slots shouldn't be empty here!");

    CCObject* tmpObj;
    Slot* slot;

    // use the first object to determie the size we use to calculate the
    // positions on the screen. This is fine as all slots have the same size.
    tmpObj = slots->objectAtIndex(0);
    slot = static_cast<Slot*>(tmpObj);
    CCSize slotSize = slot->getSize();

    for (int y = 0; y < size.height; ++y) {
        for (int x = 0; x < size.width; ++x) {

            // simply map the 2D coords to the one dimension of the CCArray
            tmpObj = slots->objectAtIndex((y * size.height) + x);
            slot = static_cast<Slot*>(tmpObj);

            slot->gridIndex = CCPoint(x, y);
            slot->setPositionX(slotSize.width * x);
            slot->setPositionY(slotSize.height * (size.height - 1 - y));
        }
    }
}

void Board::clearAllSlotsAfter(Slot* slot) const
{
    bool removeFollowing = false;
    CCArray* removeObjects = CCArray::create();
    
    CCObject* it;
    CCARRAY_FOREACH(userPath, it) {
        Slot* compareSlot = static_cast<Slot*>(it);

        if (removeFollowing) {
            compareSlot->reset();
            removeObjects->addObject(compareSlot);
        } else if (compareSlot == slot) {
            removeFollowing = true;
        }
    }

    userPath->removeObjectsInArray(removeObjects);
    userPath->reduceMemoryFootprint();
}

void Board::activateNextCheckpoint()
{
    CCObject* it = NULL;
    Slot* slot = NULL;
    lastCheckpoint = NULL;
    allCheckpointVisited = true;

    // detect the checkpoint with the highest number on the users path.
    // why? because we simply don't trust lastCheckpoint here. it should be
    // safe but it's just to simple to do a "full mind reset" here :)

    int lastNumber = 0;
    
    if (userPath->count() > 1) {
        CCARRAY_FOREACH(userPath, it) {
            slot = static_cast<Slot*>(it);
            if (slot->isCheckpoint() && slot->getNumber() > lastNumber) {
                lastCheckpoint = slot;
                lastNumber = slot->getNumber();
            }
        }
    }

    // just iterate through all slots and disable all checkpoints except the
    // one with the next number. pretty trivial but important for our
    // "full mind reset".
    
    int nextNumber = lastNumber + 1;
    bool nextCheckpointFound = false;
    numFreeSlots = 0;

    CCARRAY_FOREACH(slots, it) {
        slot = static_cast<Slot*>(it);

        if (slot->isFree()) {
            ++numFreeSlots;
        }

        if (!slot->isCheckpoint()) {
            continue;
        }

        if (!nextCheckpointFound && slot->getNumber() == nextNumber) {
            nextCheckpointFound = true;
            slot->markAsNextSlot(true);
            allCheckpointVisited = false;
        } else {
            slot->markAsNextSlot(false);
        }
    }

    // detect the game finished state and act accordingly. we don't need to
    // do anything special with all checkpoints here, because the Slot class
    // takes care of stopping all animations for us.
    
    if (allCheckpointVisited) {
        handleAllCheckpointsVisited();
    }
}

void Board::handleAllCheckpointsVisited()
{
    removeChild(touchIndicator);
    touchIndicator = NULL;
}

void Board::lockCompleteLines() const
{
    bool checkpointFound = false;

    CCObject* it = NULL;
    CCARRAY_FOREACH_REVERSE(userPath, it) {
        Slot* slot = static_cast<Slot*>(it);

        if (!checkpointFound && slot->isCheckpoint()) {
            checkpointFound = true;
        }

        if (checkpointFound) {
            slot->lock(true);
        } else {
            slot->lock(false);
        }
    }
}

Slot* Board::getLastUserPathSlot() const
{
    if (userPath->count() == 0) {
        return NULL;
    }
    
    return static_cast<Slot*>(userPath->lastObject());
}

Slot* Board::getUserPathSlotBefore(const Slot* slot) const
{
    Slot* lastSlot = NULL;

    CCObject* it = NULL;
    CCARRAY_FOREACH(userPath, it) {
        Slot* compareSlot = static_cast<Slot*>(it);

        if (compareSlot == slot) {
            return lastSlot;
        } else {
            lastSlot = compareSlot;
        }
    }

    return NULL;
}

#pragma mark Touch helper

bool Board::isFirstCheckpoint(const Slot* slot) const
{
    if (!slot->isCheckpoint()) {
        return false;
    }

    if (userPath->count() == 0) {
        return true;
    }

    Slot* lastSlot = static_cast<Slot*>(userPath->lastObject());
    if (lastSlot == slot) {
        return true;
    }

    return false;
}

void Board::createTouchIndicator()
{
    if (touchIndicator) {
        return;
    }

    touchIndicator = CCSprite::createWithSpriteFrameName("touchindicator.png");
    touchIndicator->runAction(CCRepeatForever::create(CCBlink::create(0.5, 1)));
    touchIndicator->setZOrder(BOARD_ZORDER_TOUCH_INDICATOR);
    touchIndicator->setColor(ccRED);

    addChild(touchIndicator);
}

int Board::convert2dTo1dIndex(const CCPoint grid) const
{
    return (grid.y * size.height) + grid.x;
}

CCPoint Board::get2dIndexFromPoint(const CCPoint point) const
{
    assert(slots->count() > 0 && "slots shoudn't be empty");

    Slot* firstSlot = static_cast<Slot*>(slots->objectAtIndex(0));
    int x = point.x / firstSlot->getSize().width;
    int y = point.y / firstSlot->getSize().height;

    return CCPoint(x, size.height - y - 1);
}

Slot* Board::getSlotFromPoint(const CCPoint point) const
{
    CCPoint gridIdx = get2dIndexFromPoint(point);
    int idx = convert2dTo1dIndex(gridIdx);

    if (idx < 0 || idx >= slots->count()) {
        return NULL;
    }

    return static_cast<Slot*>(slots->objectAtIndex(idx));
}