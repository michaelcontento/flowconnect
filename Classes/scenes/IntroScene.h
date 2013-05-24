#ifndef __FlowConnect__IntroScene__
#define __FlowConnect__IntroScene__

#include "cocos2d.h"

class IntroScene : public cocos2d::CCLayer
{
public:
    IntroScene();
    virtual ~IntroScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(IntroScene);
    virtual bool init() override;

    void loadResources();

private:
    void initPayment();
    void initAds();
    void initLocalization();
};

#endif /* defined(__FlowConnect__IntroScene__) */
