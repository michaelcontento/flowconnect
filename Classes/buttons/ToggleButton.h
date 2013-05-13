#ifndef __FlowConnect__ToggleButton__
#define __FlowConnect__ToggleButton__

#include "cocos2d.h"

class ToggleButton : public cocos2d::CCMenuItem
{
public:
    ToggleButton();
    virtual ~ToggleButton();

    static ToggleButton* create(const char* text, const char* onText, const char* offText, cocos2d::CCObject* target, const cocos2d::SEL_MenuHandler selector);
    virtual bool init(const char* text, const char* onText, const char* offText, cocos2d::CCObject* target, const cocos2d::SEL_MenuHandler selector);

    void setMode(const bool flag);
    void onClick();

private:
    cocos2d::CCObject* target;
    cocos2d::SEL_MenuHandler selector;
    cocos2d::CCSprite* border;
    cocos2d::CCLabelTTF* label;
    cocos2d::CCLabelTTF* onLabel;
    cocos2d::CCLabelTTF* offLabel;
    bool mode;

    void addBackground();
    void addLabel(const char* text, const char* onText, const char* offText);
    void updateStatusLabel();
};

#endif /* defined(__FlowConnect__ToggleButton__) */
