#include "LevelLoader.h"

using namespace cocos2d;

LevelLoader::LevelLoader(const char* filename)
: categories()
, idCounter(0)
, currentCategory(NULL)
, currentPage(NULL)
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
}

void LevelLoader::addLevel(const std::string& data)
{
    assert(currentPage && "page record must come first");

    LoaderLevel* level = new LoaderLevel();
    currentPage->levels.push_back(level);

    level->uid = ++idCounter;
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