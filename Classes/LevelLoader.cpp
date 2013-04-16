#include "LevelLoader.h"

using namespace cocos2d;

#pragma mark Initialisation

LevelLoader::LevelLoader(const char* filename)
: categories()
, idCounter(0)
, pageLocalCounter(0)
, currentCategory(NULL)
, currentPage(NULL)
{
    loadLevel(filename);
}

LevelLoader::~LevelLoader()
{
    for (auto category : categories) {
        for (auto page : category->pages) {
            for (auto level : page->levels) {
                level->page = NULL;
                free((char*)level->data);
                delete level;
            }

            page->levels.clear();
            page->category = NULL;
            free((char*)page->name);
            delete page;
        }

        category->pages.clear();
        free((char*)category->name);
        delete category;
    }
}

#pragma mark Category API

bool LevelLoader::hasNext(const LoaderCategory* category) const
{
    return (getNext(category) != NULL);
}

LoaderCategory* LevelLoader::getNext(const LoaderCategory* category) const
{
    bool returnNext = false;
    for (auto checkCategory : categories) {
        if (returnNext) {
            return checkCategory;
        }

        if (category->uid == checkCategory->uid) {
            returnNext = true;
        }
    }

    return NULL;
}

bool LevelLoader::hasPrevious(const LoaderCategory* category) const
{
    return (getPrevious(category) != NULL);
}

LoaderCategory* LevelLoader::getPrevious(const LoaderCategory* category) const
{
    LoaderCategory* lastCategory = NULL;
    for (auto checkCategory : categories) {
        if (category->uid == checkCategory->uid) {
            return lastCategory;
        }

        lastCategory = checkCategory;
    }

    return NULL;
}

#pragma mark Page API

bool LevelLoader::hasNext(const LoaderPage* page) const
{
    return (getNext(page) != NULL);
}

LoaderPage* LevelLoader::getNext(const LoaderPage* page) const
{
    bool returnNext = false;
    for (auto checkPage : page->category->pages) {
        if (returnNext) {
            return checkPage;
        }

        if (page->uid == checkPage->uid) {
            returnNext = true;
        }
    }

    return NULL;
}

bool LevelLoader::hasPrevious(const LoaderPage* page) const
{
    return (getPrevious(page) != NULL);
}

LoaderPage* LevelLoader::getPrevious(const LoaderPage* page) const
{
    LoaderPage* lastPage = NULL;
    for (auto checkPage : page->category->pages) {
        if (page->uid == checkPage->uid) {
            return lastPage;
        }

        lastPage = checkPage;
    }
    
    return NULL;
}

#pragma mark Level API

bool LevelLoader::hasNext(const LoaderLevel* level) const
{
    return (getNext(level) != NULL);
}

LoaderLevel* LevelLoader::getNext(const LoaderLevel* level) const
{
    bool returnNext = false;
    for (auto checkLevel : level->page->levels) {
        if (returnNext) {
            return checkLevel;
        }

        if (level->uid == checkLevel->uid) {
            returnNext = true;
        }
    }

    return NULL;
}

bool LevelLoader::hasPrevious(const LoaderLevel* level) const
{
    return (getPrevious(level) != NULL);
}

LoaderLevel* LevelLoader::getPrevious(const LoaderLevel* level) const
{
    LoaderLevel* lastLevel = NULL;
    for (auto checkLevel : level->page->levels) {
        if (level->uid == checkLevel->uid) {
            return lastLevel;
        }

        lastLevel = checkLevel;
    }

    return NULL;
}

#pragma mark Loading

void LevelLoader::loadLevel(const char* filename)
{
    std::istringstream stream(getFileContent(filename));
    std::string line;

    while (std::getline(stream, line)) {
        if (line.empty()) {
            continue;
        }

        if (line.find("#") != std::string::npos) {
            continue;
        }

        if (strcmp(&line.at(3), "!") == 0) {
            CCLog("LevelLoader: No type delimiter: %s", line.c_str());
            continue;
        }

        std::string type = line.substr(0, 3);
        std::string data = line.substr(4, std::string::npos);

        if (type.compare("cat") == 0) {
            addCategory(data);
        } else if (type.compare("pag") == 0) {
            addPage(data);
        } else if (type.compare("lvl") == 0) {
            addLevel(data);
        } else {
            CCLog("LevelLoader: Invalid type: %s", line.c_str());
        }
    }
}

void LevelLoader::addCategory(const std::string& data)
{
    currentCategory = new LoaderCategory();
    categories.push_back(currentCategory);

    currentCategory->uid = ++idCounter;
    currentCategory->name = strdup(data.c_str());
}

void LevelLoader::addPage(const std::string& data)
{
    assert(currentCategory && "category record must come first");

    currentPage = new LoaderPage();
    currentCategory->pages.push_back(currentPage);

    currentPage->uid = ++idCounter;
    currentPage->name = strdup(data.c_str());
    currentPage->category = currentCategory;

    pageLocalCounter = 0;
}

void LevelLoader::addLevel(const std::string& data)
{
    assert(currentPage && "page record must come first");

    LoaderLevel* level = new LoaderLevel();
    currentPage->levels.push_back(level);

    level->uid = ++idCounter;
    level->localid = ++pageLocalCounter;
    level->data = strdup(data.c_str());
    level->page = currentPage;
}

std::string LevelLoader::getFileContent(const char* filename)
{
    std::string fullPath = CCFileUtils::sharedFileUtils()
        ->fullPathForFilename(filename);

    if (strcmp(fullPath.c_str(), filename) == 0) {
        CCLog("LevelLoader: Unable to load file: %s", filename);
        return std::string("");
    }

    unsigned long size = 0;
    unsigned char* tmpContent = NULL;
    tmpContent = CCFileUtils::sharedFileUtils()
        ->getFileData(fullPath.c_str( ), "rb", &size);

    std::string content;
    content.assign(tmpContent, tmpContent + size - 1);

    return content;
}