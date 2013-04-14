#ifndef __FlowConnect__Colors__
#define __FlowConnect__Colors__

#include "cocos2d.h"
#include "CCGL.h"

static const GLubyte ENABLED_OPACITY = 255;
static const GLubyte DISABLED_OPACITY = 178;

static const cocos2d::ccColor3B SLOT_NUMBER_COLOR = cocos2d::ccWHITE;
static const GLubyte SLOT_NUMBER_OPACITY = ENABLED_OPACITY;

static const cocos2d::ccColor3B TOUCH_INDICATOR_COLOR = cocos2d::ccRED;
static const GLubyte TOUCH_INDICATOR_OPACITY = DISABLED_OPACITY;

static const cocos2d::ccColor3B LINE_COLORS[] = {
    cocos2d::ccRED,
    cocos2d::ccYELLOW,
    cocos2d::ccBLUE,
    cocos2d::ccMAGENTA,
    cocos2d::ccORANGE,
    cocos2d::ccGREEN,
    cocos2d::ccGRAY,
    cocos2d::ccWHITE
};

#endif /* defined(__FlowConnect__Colors__) */
