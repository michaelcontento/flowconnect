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
    unsigned int localid;
    const char* data;
    LoaderPage* page;
};

class LevelLoader
{
public:
    LevelLoader(const char* filename);
    virtual ~LevelLoader();

    CC_SYNTHESIZE_READONLY(std::vector<LoaderCategory*>, categories, Categories);

    bool hasNext(const LoaderCategory* category) const;
    bool hasPrevious(const LoaderCategory* category) const;
    LoaderCategory* getNext(const LoaderCategory* category) const;
    LoaderCategory* getPrevious(const LoaderCategory* category) const;

    bool hasNext(const LoaderPage* page) const;
    bool hasPrevious(const LoaderPage* page) const;
    LoaderPage* getNext(const LoaderPage* page) const;
    LoaderPage* getPrevious(const LoaderPage* page) const;
    
    bool hasNext(const LoaderLevel* level) const;
    bool hasPrevious(const LoaderLevel* level) const;
    LoaderLevel* getNext(const LoaderLevel* level) const;
    LoaderLevel* getPrevious(const LoaderLevel* level) const;

private:
    std::string getFileContent(const char* filename);
    unsigned int idCounter;
    unsigned int pageLocalCounter;
    LoaderCategory* currentCategory;
    LoaderPage* currentPage;

    void loadLevel(const char* filename);
    void addCategory(const std::string& data);
    void addPage(const std::string& data);
    void addLevel(const std::string& data);
};

#endif /* defined(__FlowConnect__LevelLoader__) */
