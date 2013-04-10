#include "Board.h"

#include "Token.h"
#include "Slot.h"

using namespace cocos2d;

#pragma mark Initialization

Board::Board()
: directions()
, slots(NULL)
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

    return true;
}

bool Board::initWithLevel(const cocos2d::CCSize newSize, const char* data)
{
    size = newSize;

    removeAllSlots();
    createSlotsFromData(data);
    rearrangeSlots();

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
            ++nextNumber;
        }
    }

    // free unused memory
    slots->reduceMemoryFootprint();

    assert(!nextSlotWithNumber && "nextTokenWithNumber flag still active");
}

void Board::rearrangeSlots()
{
    assert(slots->count() > 0);

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

            slot->setPositionX(slotSize.width * x);
            slot->setPositionY(slotSize.height * (size.height - 1 - y));
        }
    }
}