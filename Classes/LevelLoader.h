#ifndef __FlowConnect__LevelLoader__
#define __FlowConnect__LevelLoader__

#include "cocos2d.h"

struct LoaderPage;
struct LoaderLevel;

struct LoaderCategory {
    unsigned int uid;
    unsigned int localid;
    LoaderCategory* prev;
    LoaderCategory* next;

    const char* name;
    std::vector<LoaderPage*> pages;
};

struct LoaderPage {
    unsigned int uid;
    unsigned int localid;
    LoaderPage* prev;
    LoaderPage* next;

    const char* name;
    std::vector<LoaderLevel*> levels;
    LoaderCategory* category;
};

struct LoaderLevel {
    unsigned int uid;
    unsigned int localid;
    LoaderLevel* prev;
    LoaderLevel* next;
    
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
    unsigned int idCounter;
    unsigned int categoryLocalCounter;
    unsigned int pageLocalCounter;
    unsigned int levelLocalCounter;
    LoaderCategory* currentCategory;
    LoaderPage* currentPage;

    std::string getFileContent(const char* filename);
    
    void loadLevel(const char* filename);
    void addCategory(const std::string& data);
    void addPage(const std::string& data);
    void addLevel(const std::string& data);
};

#endif /* defined(__FlowConnect__LevelLoader__) */
