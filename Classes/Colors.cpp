#include "Colors.h"

#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

#define COLOR_STEP_WIDTH (0.618033988749895 * 360)

ccColor3B nextColor()
{
    static double lastColor = COLOR_STEP_WIDTH * 6;
    lastColor += COLOR_STEP_WIDTH;
    while (lastColor > 360) {
        lastColor -= 360;
    }
    
    HSV hsv = {lastColor, 0.9, 0.9};
    RGBA rgb = CCControlUtils::RGBfromHSV(hsv);
    return ccc3(rgb.r * 255, rgb.g * 255, rgb.b * 255);
}

const ccColor3B LINE_COLORS[] = {
    nextColor(),
    nextColor(),
    nextColor(),
    nextColor(),
    nextColor(),
    nextColor(),
    nextColor(),
    nextColor(),
    nextColor(),
    nextColor(),
    nextColor(),
    nextColor(),
    nextColor(),
    nextColor(),
    nextColor(),
    nextColor(),
    nextColor(),
    nextColor(),
    nextColor(),
    nextColor()
};