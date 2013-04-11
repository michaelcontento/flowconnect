#include "Slot.h"

using namespace cocos2d;

CCSize Slot::getSize()
{
    // TODO: use the size from spriteChild
    return CCSize(150, 150);
}

#pragma mark Initialization

Slot::Slot()
: gridIndex(-1, -1)
, isLocked_(false)
, isNextSlot_(false)
, background(NULL)
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

    background = CCSprite::createWithSpriteFrameName("backgrounds/slot.png");
    background->setPositionX(getSize().width / 2);
    background->setPositionY(getSize().height / 2);
    background->setZOrder(SLOT_ZORDER_BACKGROUND);
    addChild(background);

    lineLayer = CCLayer::create();
    lineLayer->setPositionX(getSize().width / 2);
    lineLayer->setPositionY(getSize().height / 2);
    lineLayer->setZOrder(SLOT_ZORDER_LINE_LAYER);
    addChild(lineLayer);

    return true;
}

#pragma mark Current state

void Slot::lock(const bool flag)
{
    bool changed = (isLocked_ != flag);
    isLocked_ = flag;

    if (changed) {
        updateLineImage();
    }
}

bool Slot::isLocked() const
{
    return isLocked_;
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
    lock(false);

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

    addLineImage(lineIn);
    addLineImage(lineOut);

    if (lineIn != SlotLineType::NONE && lineOut != SlotLineType::NONE) {
        addLineImage(SlotLineType::NONE);
    }
}

void Slot::addLineImage(const SlotLineType::Enum type)
{
    std::ostringstream strId;
    strId << type;
    std::string name = "lines/" + strId.str() + ".png";

    CCSprite* line = CCSprite::createWithSpriteFrameName(name.c_str());
    lineLayer->addChild(line);

    if (isLocked()) {
        line->setColor(ccGRAY);
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

        char buf[5] = {0};
        snprintf(buf, sizeof(buf), "%d", newNumber);
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
        labelBackground = CCSprite::createWithSpriteFrameName("backgrounds/slot-number.png");
        labelBackground->setPositionX(getSize().width / 2);
        labelBackground->setPositionY(getSize().height / 2);
        labelBackground->setZOrder(SLOT_ZORDER_LABEL_BACKGROUND);
        addChild(labelBackground);

        labelPulseAction = CCRepeatForever::create(
            CCSequence::create(
                CCScaleTo::create(0.125, 1.05),
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
        label = CCLabelTTF::create("", "Markler Fett", 48);
        label->setPositionX(getSize().width / 2);
        label->setPositionY(getSize().height / 2);
        label->setColor(ccGREEN);
        label->setZOrder(SLOT_ZORDER_LABEL);
        addChild(label);
    }
}