#include "Board.h"

#include "Token.h"
#include "../Colors.h"
#include "userstate.h"

using namespace cocos2d;

#pragma mark Initialization

Board::Board()
: directions()
, slots(NULL)
, userPath(NULL)
, level(NULL)
, lastCheckpoint(NULL)
, touchIndicator(NULL)
, allCheckpointVisited(false)
, numFreeSlots(0)
, moves(0)
, progress(0)
, duration(0)
, timerStarted(false)
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

    return true;
}

bool Board::initWithLevel(const LoaderLevel* level)
{
    assert(level && "null pointer");

    removeAllSlots();
    createSlotsFromData(level->data);
    slots->reduceMemoryFootprint();

    float sqrtSize = sqrtf(slots->count());
    assert((float)(int)sqrtSize == sqrtSize && "grid must be quadratic");

    size = CCSize(sqrtSize, sqrtSize);
    this->level = level;
    numFreeSlots = slots->count();

    positionSlotsOnScreen();

    return true;
}

void Board::onEnter()
{
    CCLayer::onEnter();
    
    CCDirector::sharedDirector()
        ->getTouchDispatcher()
        ->addTargetedDelegate(this, 0, true);

    if (timerStarted) {
        schedule(schedule_selector(Board::updateDuration));
    }
}

void Board::onExit()
{
    CCLayer::onExit();

    CCDirector::sharedDirector()
        ->getTouchDispatcher()
        ->removeDelegate(this);

    if (timerStarted) {
        unschedule(schedule_selector(Board::updateDuration));
    }
}

#pragma mark Touch handling

bool Board::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isFinished()) {
        return false;
    }
    
    const CCPoint touchPos = convertTouchToNodeSpace(pTouch);
    if (!boundingBox().containsPoint(touchPos)) {
        return false;
    }

    Slot* slot = getSlotFromPoint(touchPos);
    if (!slot){
        return false;
    }

    if (slot->isCheckpoint()) {
        if ((isFirstCheckpoint(slot) && slot->isFree()) || !slot->isFree()) {
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

    createTouchIndicator();
    stopTouchIndicatorBlink();

    if (slot->isCheckpoint()) {
        touchIndicator->setColor(LINE_COLORS[slot->getNumber() - 1]);
    } else {
        touchIndicator->setColor(slot->getColor());
    }
    touchIndicator->setPosition(touchPos);

    int slotDiff = userPath->count();

    clearAllSlotsAfter(slot);
    appendUserPath(slot);

    slotDiff -= userPath->count();
    if (slot->getNumber() == 1) {
        moves = 0;
    } else {
        moves += slotDiff;
    }

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
    if (!boundingBox().containsPoint(touchPos)) {
        return;
    }

    Slot* currentSlot = getSlotFromPoint(touchPos);
    if (!currentSlot) {
        return;
    }

    Slot* lastSlot = getLastUserPathSlot();
    assert(lastSlot && "should always be there because of ccTouchBegin");

    // ----- Validate movement

    if (currentSlot == lastSlot) {
        touchIndicator->setPosition(touchPos);
        return;
    }

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
        if (currentSlot->isFree()) {
            // we're allowed to move into free slots
        } else if (currentSlot == getUserPathSlotBefore(lastSlot)) {
            // alloweed bcause we're waling backwards
        } else {
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

    startGameTimer();
}

void Board::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (userPath->count() <= 1) {
        userPath->removeAllObjects();
        activateNextCheckpoint();
    }

    startTouchIndicatorBlink();
}

void Board::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    ccTouchEnded(pTouch, pEvent);
}

#pragma mark Slot handling

void Board::updateDuration(float dt)
{
    duration += dt;
}

bool Board::isFinished() const
{
    return (allCheckpointVisited && numFreeSlots == 0);
}

void Board::appendUserPath(Slot* slot)
{
    Slot* lastSlot = getLastUserPathSlot();
    if (lastSlot != slot) {
        if (lastCheckpoint) {
            slot->setColor(LINE_COLORS[lastCheckpoint->getNumber() - 1]);
        } else if (lastSlot && lastSlot->isCheckpoint()) {
            slot->setColor(LINE_COLORS[lastSlot->getNumber() - 1]);
        } else {
            assert(false && "there must always be one checkpoint");
        }

        userPath->addObject(slot);
    }

    if (touchIndicator) {
        if (slot->isCheckpoint()) {
            touchIndicator->setColor(LINE_COLORS[slot->getNumber() - 1]);
        } else {
            touchIndicator->setColor(slot->getColor());
        }
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

        if (!token::isDirectionToken(currentToken[0])) {
            nextNumber = (nextNumber * 10) + atoi(currentToken);
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
        directions.push_back(data[i]);
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
            slot->setZOrder(-convert2dTo1dIndex(slot->gridIndex));
            slot->setPositionX(slotSize.width * x);
            slot->setPositionY(slotSize.height * (size.height - 1 - y));
        }
    }

    setContentSize(CCSize(slotSize.width * size.width, slotSize.height * size.height));
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

Slot* Board::activateNextCheckpoint()
{
    CCObject* it = NULL;
    Slot* slot = NULL;
    lastCheckpoint = NULL;
    allCheckpointVisited = true;

    // detect the checkpoint with the highest number on the users path.
    // why? because we simply don't trust lastCheckpoint here. it should be
    // safe but it's just to simple to do a "full mind reset" here :)

    int lastNumber = 0;
    
    CCARRAY_FOREACH(userPath, it) {
        slot = static_cast<Slot*>(it);

        if (slot->isCheckpoint() && slot->getNumber() > lastNumber) {
            lastCheckpoint = slot;
            lastNumber = slot->getNumber();
        }
    }

    // just iterate through all slots and disable all checkpoints except the
    // one with the next number. pretty trivial but important for our
    // "full mind reset".
    
    int nextNumber = lastNumber + 1;
    Slot* nextCheckpoint = NULL;
    numFreeSlots = 0;

    CCARRAY_FOREACH(slots, it) {
        slot = static_cast<Slot*>(it);

        if (slot->isFree()) {
            ++numFreeSlots;
        }

        if (!slot->isCheckpoint()) {
            continue;
        }

        if (!nextCheckpoint && slot->getNumber() == nextNumber) {
            nextCheckpoint = slot;
            slot->markAsNextSlot(true);
            allCheckpointVisited = false;
        } else {
            slot->markAsNextSlot(false);
        }
    }

    int totalSlots = size.width * size.height;
    progress = (totalSlots - numFreeSlots) / float(totalSlots);

    // detect the game finished state and act accordingly. we don't need to
    // do anything special with all checkpoints here, because the Slot class
    // takes care of stopping all animations for us.

    if (allCheckpointVisited) {
        handleAllCheckpointsVisited();
    }

    return nextCheckpoint;
}

void Board::handleAllCheckpointsVisited()
{
    removeChild(touchIndicator);
    touchIndicator = NULL;

    auto lastState = userstate::getModeForLevel(level);
    auto perfectMoves = (size.width * size.height) - 1;
    if (moves == perfectMoves || lastState == userstate::Mode::PERFECT) {
        userstate::setModeForLevel(level, userstate::Mode::PERFECT);
    } else {
        userstate::setModeForLevel(level, userstate::Mode::SOLVED);
    }

    unschedule(schedule_selector(Board::updateDuration));
}

void Board::lockCompleteLines() const
{
    int checkpointCounter = 0;

    CCObject* it = NULL;
    CCARRAY_FOREACH_REVERSE(userPath, it) {
        Slot* slot = static_cast<Slot*>(it);

        if (slot->isCheckpoint()) {
            ++checkpointCounter;
        }

        if (checkpointCounter > 0) {
            slot->lockLineIn(true);
            slot->lockLineOut(true);

            if (checkpointCounter == 1 && slot->isCheckpoint()) {
                slot->lockLineOut(false);
            }
        } else {
            slot->lockLineIn(false);
            slot->lockLineOut(false);
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

void Board::reset()
{
    if (userPath->count() > 0) {
        Slot* firstSlot = static_cast<Slot*>(userPath->objectAtIndex(0));
        clearAllSlotsAfter(firstSlot);

        assert(userPath->count() == 1 && "something went wrong");

        userPath->removeAllObjects();
        firstSlot->reset();
    }

    unschedule(schedule_selector(Board::updateDuration));
    timerStarted = false;
    duration = 0;
    moves = 0;

    if (touchIndicator) {
        stopTouchIndicatorBlink();
        touchIndicator->setVisible(false);
    }

    activateNextCheckpoint();
}

#pragma mark TouchIndicator

void Board::startTouchIndicatorBlink()
{
    if (!touchIndicator) {
        return;
    }

    touchIndicator->runAction(
        CCRepeatForever::create(CCBlink::create(1, 1))
    );
}

void Board::stopTouchIndicatorBlink()
{
    if (!touchIndicator) {
        return;
    }
    
    touchIndicator->stopAllActions();
    touchIndicator->setVisible(true);
}

void Board::createTouchIndicator()
{
    if (touchIndicator) {
        return;
    }

    touchIndicator = CCSprite::createWithSpriteFrameName("slot/touchindicator.png");
    touchIndicator->setZOrder(BOARD_ZORDER_TOUCH_INDICATOR);
    touchIndicator->setOpacity(TOUCH_INDICATOR_OPACITY);
    touchIndicator->setScale(2);

    addChild(touchIndicator);
}

#pragma mark Touch helper

bool Board::isFirstCheckpoint(const Slot* slot) const
{
    if (!slot->isCheckpoint()) {
        return false;
    }

    if (slot->getNumber() == 1) {
        return true;
    }

    return false;
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

bool Board::finishTillNextCheckpoint()
{
    // nothing to do here
    if (allCheckpointVisited) {
        return false;
    }
    
    // detect the first checkpoint
    Slot* firstCheckpoint = NULL;
    if (userPath->count() > 0) {
        firstCheckpoint = dynamic_cast<Slot*>(userPath->objectAtIndex(0));
    } else if (!firstCheckpoint) {
        firstCheckpoint = activateNextCheckpoint();
    }
    assert(firstCheckpoint && "unable to detect first checkpoint");
    assert(isFirstCheckpoint(firstCheckpoint) && "we need to start at the first checkpoint");

    // detect the checkpoint we should end at
    int endCheckpointNumber = 0;
    if (lastCheckpoint) {
        endCheckpointNumber = lastCheckpoint->getNumber() + 1;
    } else {
        endCheckpointNumber = firstCheckpoint->getNumber() + 1;
    }
    assert(endCheckpointNumber > 1 && "we should - at least - start with the second checkpoint");

    // store which slots are used -- important to detect the right amount
    // of moves later!
    auto usedSlots = std::set<int>();
    CCObject* it = NULL;
    Slot* itSlot = NULL;
    CCARRAY_FOREACH(userPath, it) {
        itSlot = static_cast<Slot*>(it);
        if (!itSlot->isFree()) {
            usedSlots.insert(convert2dTo1dIndex(itSlot->gridIndex));
        }
    }

    // clear the old path
    lastCheckpoint = firstCheckpoint;
    clearAllSlotsAfter(firstCheckpoint);

    // disable the touch indicator -- it would be at the wrong position ...
    if (touchIndicator) {
        stopTouchIndicatorBlink();
        touchIndicator->setVisible(false);
    }

    // some vars we need for the next loop. the following code should be
    // refactored to .. :)
    Slot* currentSlot = firstCheckpoint;
    int nextSlotIndex = convert2dTo1dIndex(currentSlot->gridIndex);
    Slot* nextSlot = NULL;
    CCPoint nextGridIndex;
    SlotLineType::Enum lineOut;
    SlotLineType::Enum lineIn;

    while (true) {
        // detect the next position in the grid
        nextGridIndex = currentSlot->gridIndex;
        if (directions.at(nextSlotIndex) == TOKEN_DIRECTION_LEFT) {
            nextGridIndex.x -= 1;
            lineOut = SlotLineType::LEFT;
            lineIn = SlotLineType::RIGHT;
        } else if (directions.at(nextSlotIndex) == TOKEN_DIRECTION_RIGHT) {
            nextGridIndex.x += 1;
            lineOut = SlotLineType::RIGHT;
            lineIn = SlotLineType::LEFT;
        } else if (directions.at(nextSlotIndex) == TOKEN_DIRECTION_UP) {
            nextGridIndex.y -= 1;
            lineOut = SlotLineType::TOP;
            lineIn = SlotLineType::BOTTOM;
        } else if (directions.at(nextSlotIndex) == TOKEN_DIRECTION_DOWN) {
            nextGridIndex.y += 1;
            lineOut = SlotLineType::BOTTOM;
            lineIn = SlotLineType::TOP;
        }

        // convert the grid position to the slot
        nextSlotIndex = convert2dTo1dIndex(nextGridIndex);
        nextSlot = dynamic_cast<Slot*>(slots->objectAtIndex(nextSlotIndex));
        assert(nextSlot && "sould not be null");

        // update the lines on both slots
        if (!usedSlots.count(nextSlotIndex)) {
            ++moves;
        }
        currentSlot->setLineOut(lineOut);
        nextSlot->setLineIn(lineIn);

        // update the user path
        appendUserPath(currentSlot);
        if (nextSlot->isCheckpoint() && nextSlot->getNumber() == endCheckpointNumber) {
            appendUserPath(nextSlot);
            break;
        }

        // advance our slot pointers
        currentSlot = nextSlot;
        nextSlot = NULL;
    }

    startGameTimer();

    return true;
}

void Board::startGameTimer()
{
    if (!timerStarted) {
        timerStarted = true;
        schedule(schedule_selector(Board::updateDuration));
    }
}