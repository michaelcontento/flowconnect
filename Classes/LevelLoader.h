#ifndef __FlowConnect__LevelLoader__
#define __FlowConnect__LevelLoader__

#include "cocos2d.h"

struct LoaderPage;
struct LoaderLevel;

struct LoaderRecord {
    unsigned int uid;
    unsigned int localid;
    const char* description;
};

struct LoaderCategory : LoaderRecord {
    LoaderCategory* prev;
    LoaderCategory* next;

    const char* name;
    std::vector<LoaderPage*> pages;

    unsigned int countLevels();
    unsigned int countLevelsSolved();
};

struct LoaderPage : LoaderRecord {
    LoaderPage* prev;
    LoaderPage* next;
    LoaderCategory* category;

    const char* name;
    std::vector<LoaderLevel*> levels;
};

struct LoaderLevel : LoaderRecord {
    LoaderLevel* prev;
    LoaderLevel* next;
    LoaderPage* page;

    const char* data;
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
    LoaderRecord* lastRecord;

    std::string getFileContent(const char* filename);
    
    void loadLevel(const char* filename);
    void addCategory(const std::string& data);
    void addPage(const std::string& data);
    void addLevel(const std::string& data);
    void addDescription(const std::string& data);
};

#endif /* defined(__FlowConnect__LevelLoader__) */
