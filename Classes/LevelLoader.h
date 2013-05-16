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
    const char* getLocalizedName() const;
    const char* getLocalizedDescription() const;
};

struct LoaderPage : LoaderRecord {
    LoaderPage* prev;
    LoaderPage* next;
    LoaderCategory* category;

    const char* name;
    std::vector<LoaderLevel*> levels;

    const char* getLocalizedName() const;
};

struct LoaderLevel : LoaderRecord {
    LoaderLevel* prev;
    LoaderLevel* next;
    LoaderPage* page;

    const char* data;
    
    const char* getLocalizedName() const;
};

class LevelLoader
{
public:
    LevelLoader();
    virtual ~LevelLoader();

    void loadLevel(const char* filename);

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
    
    void addCategory(const std::string& data);
    void addPage(const std::string& data);
    void addLevel(const std::string& data);
    void addDescription(const std::string& data);
};

#endif /* defined(__FlowConnect__LevelLoader__) */
