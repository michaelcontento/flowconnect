#include "Slot.h"

using namespace cocos2d;

CCSize Slot::getSize()
{
    // TODO: use the size from spriteChild
    return CCSize(150, 150);
}

#pragma mark Initialization

Slot::Slot()
: isFinal_(false)
, background(NULL)
, line(NULL)
, label(NULL)
, labelBackground(NULL)
{
}

Slot::~Slot()
{
}

bool Slot::init()
{
    if (!CCNode::init()) {
        return false;
    }

    background = CCSprite::createWithSpriteFrameName("backgrounds/slot.png");
    background->setPositionX(getSize().width / 2);
    background->setPositionY(getSize().height / 2);
    addChild(background);

    return true;
}

#pragma mark Current state

void Slot::removeDirection()
{
    if (line) {
        removeChild(line);
        CC_SAFE_RELEASE_NULL(line);
    }
}

void Slot::setDirection(const int direction)
{
    removeDirection();
    
    switch (direction) {
        case SLOT_DIRECTION_LEFT:
            line = CCSprite::createWithSpriteFrameName("lines/left.png");
            break;

        case SLOT_DIRECTION_RIGHT:
            line = CCSprite::createWithSpriteFrameName("lines/right.png");
            break;

        case SLOT_DIRECTION_UP:
            line = CCSprite::createWithSpriteFrameName("lines/up.png");
            break;

        case SLOT_DIRECTION_DOWN:
            line = CCSprite::createWithSpriteFrameName("lines/down.png");
            break;

        default:
            assert(false && "unknown direction given");
            break;
    }

    line->setPositionX(getSize().width / 2);
    line->setPositionY(getSize().height / 2);
    addChild(line);
}

void Slot::markAsFinal()
{
    if (isFinal_) {
        return;
    }

    isFinal_ = true;
}

bool Slot::isFinal() const
{
    return isFinal_;
}

void Slot::markAsPending()
{
    if (!isFinal_) {
        return;
    }
    
    isFinal_ = false;
}

bool Slot::isPending() const
{
    return !isFinal_;
}

#pragma mark Number handleing

void Slot::setNumber(const int newNumber)
{
    number = newNumber;

    if (newNumber == SLOT_NO_NUMBER) {
        hideNumber();
    } else {
        showNumber();

        char buf[5] = {0};
        snprintf(buf, sizeof(buf), "%d", newNumber);
        label->setString(buf);
    }
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
        addChild(labelBackground);
    }

    if (label) {
        label->setVisible(true);
    } else {
        label = CCLabelTTF::create("", "Markler Fett", 48);
        label->setPositionX(getSize().width / 2);
        label->setPositionY(getSize().height / 2);
        label->setColor(ccGREEN);
        addChild(label);
    }
}