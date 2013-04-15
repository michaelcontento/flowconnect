#ifndef __FlowConnect__LevelLoader__
#define __FlowConnect__LevelLoader__

#include "cocos2d.h"

struct LoaderPage;
struct LoaderLevel;

struct LoaderCategory {
    unsigned int uid;
    const char* name;
    std::vector<LoaderPage*> pages;
};

struct LoaderPage {
    unsigned int uid;
    const char* name;
    std::vector<LoaderLevel*> levels;
    LoaderCategory* category;
};

struct LoaderLevel {
    unsigned int uid;
    const char* data;
    LoaderPage* page;
};

class LevelLoader
{
public:
    LevelLoader(const char* filename);
    virtual ~LevelLoader();

    CC_SYNTHESIZE_READONLY(std::vector<LoaderCategory*>, categories, Categories);

private:
    std::string getFileContent(const char* filename);
    unsigned int idCounter;
    LoaderCategory* currentCategory;
    LoaderPage* currentPage;

    void addCategory(const std::string& data);
    void addPage(const std::string& data);
    void addLevel(const std::string& data);
};

#endif /* defined(__FlowConnect__LevelLoader__) */
