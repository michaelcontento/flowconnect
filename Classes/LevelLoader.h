#ifndef __FlowConnect__LevelLoader__
#define __FlowConnect__LevelLoader__

#include "cocos2d.h"
#include <vector.h>

struct LoaderPage;
struct LoaderLevel;

struct LoaderCategory {
    int uid;
    char* name;
    std::vector<LoaderPage*> pages;
};

struct LoaderPage {
    int uid;
    char* name;
    std::vector<LoaderLevel*> levels;
    LoaderCategory* category;
};

struct LoaderLevel {
    int uid;
    cocos2d::CCSize size;
    char* data;
    LoaderPage* page;
};

class LevelLoader
{
public:
    LevelLoader(const char* filename);
    virtual ~LevelLoader();

    CC_SYNTHESIZE_READONLY(std::vector<LoaderCategory*>, categories, Categories)
};

#endif /* defined(__FlowConnect__LevelLoader__) */
