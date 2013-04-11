#include "Board.h"

#include "Token.h"

using namespace cocos2d;

#pragma mark Initialization

Board::Board()
: directions()
, slots(NULL)
, lastSlot(NULL)
, lastCheckpoint(NULL)
, touchIndicator(NULL)
{
}

Board::~Board()
{
    removeAllSlots();
    CC_SAFE_RELEASE_NULL(slots);
}

bool Board::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    
    slots = CCArray::create();
    CC_SAFE_RETAIN(slots);

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
    rearrangeSlotsInArray();

    slots->reduceMemoryFootprint();

    return true;
}

#pragma mark Slots housekeeping

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

void Board::rearrangeSlotsInArray()
{
    CCArray* moveToBottom = CCArray::create();
    CCObject* it;

    CCARRAY_FOREACH(slots, it) {
        Slot* slot = static_cast<Slot*>(it);
        if (slot->isNextSlot()) {
            break;
        }

        moveToBottom->addObject(it);
    }

    CCARRAY_FOREACH(moveToBottom, it) {
        slots->removeObject(it);
        slots->addObject(it);
    }
}

void Board::activateNextCheckpoint(Slot* slot) const
{
    slot->markAsNextSlot(false);

    Slot* nextCheckpoint = getNextCheckpoint(slot);
    if (nextCheckpoint) {
        nextCheckpoint->markAsNextSlot(true);
    }
}

Slot* Board::getNextCheckpoint(const Slot* currentSlot) const
{
    bool returnNextCheckpoint = false;
    CCObject* it;

    CCARRAY_FOREACH(slots, it) {
        Slot* compareSlot = static_cast<Slot*>(it);

        if (returnNextCheckpoint && compareSlot->isCheckpoint()) {
            return compareSlot;
        }

        if (compareSlot == currentSlot) {
            returnNextCheckpoint = true;
        }
    }

    return NULL;
}

#pragma mark Touch handling

bool Board::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    const CCPoint touchPos = convertTouchToNodeSpace(pTouch);
    Slot* slot = getSlotFromPoint(touchPos);

    // ignore clicks on empty slots
    if (!slot->isCheckpoint() && slot->isFree()) {
        return false;
    }

    // handle checkpoint clicks
    if (slot->isCheckpoint()) {
        if (slot->isNextSlot() || slot->isLocked()) {
            // click on the next checkpoint OR one that is already reached
            lastCheckpoint = slot;
            activateNextCheckpoint(slot);
        } else {
            // the user tried to skip ahead to the next checkpoint
            return false;
        }
    }

    // update lasSlot, move the indicator and clear the trailing path. this has
    // to be done for checkpoint clicks and click somewhere on the existing path.
    lastSlot = slot;
    // TODO: clearAllSlotsAfter(slot);

    createTouchIndicator();
    touchIndicator->setPosition(touchPos);

    return true;
}

void Board::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    const CCPoint touchPos = convertTouchToNodeSpace(pTouch);
    Slot* currentSlot = getSlotFromPoint(touchPos);

    // ----- Validate movement

    if (currentSlot->isCheckpoint() && !currentSlot->isNextSlot()) {
        return;
    }

    CCPoint lastGridIdx = lastSlot->gridIndex;
    CCPoint currentGridIdx = get2dIndexFromPoint(touchPos);

    int distanceX = lastGridIdx.x - currentGridIdx.x;
    int distanceY = lastGridIdx.y - currentGridIdx.y;
    int distanceSum = abs(distanceX) + abs(distanceY);

    if (distanceSum == 0 || distanceSum > 1) {
        return;
    }

    // ----- Update slots

    if (distanceX == -1) {
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

    if (currentSlot->isCheckpoint()) {
        activateNextCheckpoint(currentSlot);
    }

    lastSlot = currentSlot;
    touchIndicator->setPosition(touchPos);
}

void Board::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
}

void Board::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}

#pragma mark Touch helper

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
    
    return static_cast<Slot*>(slots->objectAtIndex(idx));
}










