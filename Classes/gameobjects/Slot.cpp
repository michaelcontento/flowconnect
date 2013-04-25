#include "Slot.h"

#include "Globals.h"
#include "../Colors.h"
#include "userstate.h"

using namespace cocos2d;

CCSize Slot::getSize() const
{
    // TODO: use the size from spriteChild
    return CCSize(174, 174);
}

#pragma mark Initialization

Slot::Slot()
: gridIndex(-1, -1)
, lineInLocked(false)
, lineOutLocked(false)
, isNextSlot_(false)
, lineLayer(NULL)
, label(NULL)
, labelBackground(NULL)
, labelPulseAction(NULL)
, number(SLOT_DEFAULT_NUMBER)
, lineIn(SlotLineType::NONE)
, lineOut(SlotLineType::NONE)
{
}

Slot::~Slot()
{
    CC_SAFE_RELEASE_NULL(labelPulseAction);
}

bool Slot::init()
{
    if (!CCNode::init()) {
        return false;
    }

    lineLayer = CCLayer::create();
    lineLayer->setPositionX(getSize().width / 2);
    lineLayer->setPositionY(getSize().height / 2);
    lineLayer->setZOrder(SLOT_ZORDER_LINE_LAYER);
    addChild(lineLayer);

    return true;
}

#pragma mark Current state

void Slot::setColor(const cocos2d::ccColor3B newColor)
{
    bool changed = (color.r != newColor.r || color.g != newColor.g || color.b != newColor.b);
    color = newColor;

    if (changed) {
        updateLineImage();
    }
}

void Slot::lockLineIn(const bool flag)
{
    bool changed = (lineInLocked != flag);
    lineInLocked = flag;

    if (changed) {
        updateLineImage();
    }
}

void Slot::lockLineOut(const bool flag)
{
    bool changed = (lineOutLocked != flag);
    lineOutLocked = flag;

    if (changed) {
        updateLineImage();
    }
}

bool Slot::isLocked() const
{
    return lineOutLocked;
}

void Slot::setLineIn(const SlotLineType::Enum line)
{
    bool changed = (lineIn != line);
    lineIn = line;

    if (changed) {
        updateLineImage();
    }
}

void Slot::setLineOut(const SlotLineType::Enum line)
{
    bool changed = (lineOut != line);
    lineOut = line;

    if (changed) {
        updateLineImage();
    }
}

void Slot::markAsNextSlot(const bool flag)
{
    if (labelBackground && isNextSlot_ != flag) {
        if (flag) {
            labelBackground->runAction(labelPulseAction);
        } else {
            labelBackground->stopAllActions();
            labelBackground->setScale(1);
        }
    }

    isNextSlot_ = flag;
}

bool Slot::isNextSlot() const
{
    return isNextSlot_;
}

void Slot::reset()
{
    lineLayer->removeAllChildren();
    lockLineIn(false);
    lockLineOut(false);

    setLineIn(SlotLineType::NONE);
    setLineOut(SlotLineType::NONE);
}

bool Slot::isFree() const
{
    return (lineIn == SlotLineType::NONE && lineOut == SlotLineType::NONE);
}

void Slot::updateLineImage()
{
    lineLayer->removeAllChildren();

    if (lineIn == SlotLineType::NONE && lineOut == SlotLineType::NONE) {
        return;
    }

    // TODO: Refactor this mess .. hihi :D
    // Hint: The magic "== 1 || == -3" is used to detect if the image needs
    //       to be rotated counter- or clockwise
    
    if (lineOut == SlotLineType::NONE) {
        addLineImage(lineIn, "slot/lines/center.png", lineInLocked, getColor());
    } else if (lineIn == SlotLineType::NONE) {
        if (isCheckpoint()) {
            addLineImage(lineOut, "slot/lines/center.png", lineOutLocked, labelBackground->getColor());
        } else {
            addLineImage(lineOut, "slot/lines/center.png", lineOutLocked, getColor());
        }
    } else {
        if (isLineOpposite(lineIn, lineOut)) {
            if (isCheckpoint()) {
                addLineImage(lineIn, "slot/lines/straight-0.png", lineInLocked, getColor());
                addLineImage(lineIn, "slot/lines/straight-1.png", lineOutLocked, labelBackground->getColor());
            } else {
                addLineImage(lineIn, "slot/lines/straight.png", lineInLocked, getColor());
            }
        } else {
            if ((lineIn - lineOut) == 1 || (lineIn - lineOut) == -3) {
                if (isCheckpoint()) {
                    addLineImage(lineOut, "slot/lines/curve-0.png", lineOutLocked, labelBackground->getColor());
                    addLineImage(lineOut, "slot/lines/curve-1.png", lineInLocked, getColor());
                } else {
                    addLineImage(lineOut, "slot/lines/curve.png", lineOutLocked, getColor());
                }
            } else {
                if (isCheckpoint()) {
                    addLineImage(lineIn, "slot/lines/curve-0.png", lineInLocked, getColor());
                    addLineImage(lineIn, "slot/lines/curve-1.png", lineOutLocked, labelBackground->getColor());
                } else {
                    addLineImage(lineIn, "slot/lines/curve.png", lineInLocked, getColor());
                }
            }
        }
    }
}

bool Slot::isLineOpposite(const SlotLineType::Enum lineIn, const SlotLineType::Enum lineOut) const
{
    return ((lineIn % 2) == (lineOut % 2));
}

void Slot::addLineImage(const SlotLineType::Enum type, const char* file, const bool locked, const ccColor3B color)
{
    CCSprite* line = CCSprite::createWithSpriteFrameName(file);
    line->setColor(color);
    lineLayer->addChild(line);

    if (type == SlotLineType::RIGHT) {
        line->setRotation(90);
    } else if (type == SlotLineType::BOTTOM) {
        line->setRotation(180);
    } else if (type == SlotLineType::LEFT) {
        line->setRotation(270);
    }

    if (locked) {
        line->setOpacity(ENABLED_OPACITY);
    } else {
        line->setOpacity(DISABLED_OPACITY);
    }
}

#pragma mark Number handling

void Slot::setNumber(const int newNumber)
{
    number = newNumber;

    if (newNumber == SLOT_DEFAULT_NUMBER) {
        hideNumber();
    } else {
        showNumber();
        labelBackground->setColor(LINE_COLORS[newNumber - 1]);

        static char buf[5] = {0};
        if (userstate::isNumberMode()) {
            snprintf(buf, sizeof(buf), "%d", newNumber);
        } else {
            snprintf(buf, sizeof(buf), "%c", newNumber + 64);
        }
        label->setString(buf);
    }
}

bool Slot::isCheckpoint() const
{
    return (getNumber() != SLOT_DEFAULT_NUMBER);
}

void Slot::hideNumber()
{
    if (labelBackground) {
        labelBackground->setVisible(false);
    }

    if (label) {
        label->setVisible(false);
    }
}

void Slot::showNumber()
{
    if (labelBackground) {
        labelBackground->setVisible(true);
    } else {
        labelBackground = CCSprite::createWithSpriteFrameName("slot/ring.png");
        labelBackground->setPositionX(getSize().width / 2);
        labelBackground->setPositionY(getSize().height / 2);
        labelBackground->setZOrder(SLOT_ZORDER_LABEL_BACKGROUND);
        addChild(labelBackground);

        labelPulseAction = CCRepeatForever::create(
            CCSequence::create(
                CCScaleTo::create(0.125, 1.10),
                CCDelayTime::create(0.125),
                CCScaleTo::create(0.125, 1),
                CCDelayTime::create(2),
                NULL
            )
        );
        labelPulseAction->retain();
        
        markAsNextSlot(isNextSlot());
    }

    if (label) {
        label->setVisible(true);
    } else {
        label = CCLabelTTF::create("", DEFAULT_FONT_NAME, 82);
        label->setPositionX(getSize().width / 2);
        label->setPositionY(getSize().height / 2 + 3);
        label->setColor(SLOT_NUMBER_COLOR);
        label->setOpacity(SLOT_NUMBER_OPACITY);
        label->setZOrder(SLOT_ZORDER_LABEL);
        addChild(label);
    }
}