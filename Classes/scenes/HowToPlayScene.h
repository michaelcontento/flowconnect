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
    virtual void keyBackClicked() override;

    void btnPlay();

private:
    static const int BOARD_WIDTH = 400;

};

#endif /* defined(__FlowConnect__HowToPlayScene__) */
