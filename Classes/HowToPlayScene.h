#ifndef __FlowConnect__HowToPlayScene__
#define __FlowConnect__HowToPlayScene__

#include "cocos2d.h"

class HowToPlayScene : public cocos2d::CCLayer
{
public:
    HowToPlayScene();
    virtual ~HowToPlayScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(HowToPlayScene);
    virtual bool init() override;

    void btnPlay();
};

#endif /* defined(__FlowConnect__HowToPlayScene__) */
