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

    clearAllSlotsAfter(slot);
    appendUserPath(slot);

    createTouchIndicator();
    touchIndicator->setPosition(touchPos);

    return true;
}

void Board::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    // no checkpoint to reach? the user might a) proceed to the next level
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
            // allowed because it's the next safe point
        } else if (currentSlot == getUserPathSlotBefore(lastSlot)) {
            // allowed because we're walk back
        } else {
            return;
        }
    } else {
        if (currentSlot->isLocked() && !lastSlot->isLocked()) {
            return;
        }
    }

    // ----- Update slots

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
    if (!slots) {
        return;
    }

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

    int nextNumber = 1;
    bool nextSlotWithNumber = false;

    for (int i = 0; i < len; ++i) {
        currentToken[0] = data[i];

        if (strcmp(currentToken, TOKEN_FLAG_NUMBER) == 0) {
            nextSlotWithNumber = true;
            continue;
        }

        Slot* nextSlot = Slot::create();
        slots->addObject(nextSlot);
        addChild(nextSlot);

        if (token::isDirectionToken(currentToken)) {
            directions.push_back(token::convertTokenToSlotDirection(currentToken));
        } else {
            directions.push_back(directions.back());
        }

        if (nextSlotWithNumber) {
            nextSlotWithNumber = false;
            nextSlot->setNumber(nextNumber);

            if (nextNumber == 1) {
                nextSlot->markAsNextSlot(true);
            }
            
            ++nextNumber;
        }
    }

    assert(!nextSlotWithNumber && "nextTokenWithNumber flag still active");
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
    Slot* lastCheckpoint = NULL;

    // pretend all checkpoints were visited. to check (and flip this flag) is
    // the job of the following code.
    allCheckpointVisited = true;

    if (userPath->count() > 1) {
        CCARRAY_FOREACH(userPath, it) {
            slot = static_cast<Slot*>(it);
            if (slot->isCheckpoint()) {
                lastCheckpoint = slot;
            }
        }
    }

    bool flagNextCheckpoint = false;
    if (!lastCheckpoint) {
        flagNextCheckpoint = true;
    }

    CCARRAY_FOREACH(slots, it) {
        slot = static_cast<Slot*>(it);

        if (!slot->isCheckpoint()) {
            continue;
        }

        if (flagNextCheckpoint) {
            slot->markAsNextSlot(true);
            flagNextCheckpoint = false;
            allCheckpointVisited = false;
        } else {
            slot->markAsNextSlot(false);
        }

        if (slot == lastCheckpoint) {
            flagNextCheckpoint = true;
        }
    }

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
    CCObject* it = NULL;
    bool checkpointFound = false;

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
    CCObject* it = NULL;
    Slot* lastSlot = NULL;

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
    if (!touchIndicator) {
        touchIndicator = CCSprite::createWithSpriteFrameName("touchindicator.png");
        touchIndicator->runAction(CCRepeatForever::create(CCBlink::create(0.5, 1)));
        touchIndicator->setZOrder(BOARD_TOUCH_INDICATOR_Z_INDEX);
        touchIndicator->setColor(ccRED);

        addChild(touchIndicator);
    }
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