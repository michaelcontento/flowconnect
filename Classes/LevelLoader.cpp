#include "LevelLoader.h"

#include <avalon/i18n/Localization.h>
#include <avalon/i18n/LanguageKey.h>
#include "userstate.h"

using namespace cocos2d;
using avalon::i18n::_;

#pragma mark Initialisation

LevelLoader::LevelLoader()
: categories()
, idCounter(0)
, categoryLocalCounter(0)
, pageLocalCounter(0)
, levelLocalCounter(0)
, currentCategory(NULL)
, currentPage(NULL)
, lastRecord(NULL)
{
}

LevelLoader::~LevelLoader()
{
    for (auto category : categories) {
        for (auto page : category->pages) {
            for (auto level : page->levels) {
                level->page = NULL;
                free((char*)level->data);
                free((char*)level->description);
                delete level;
            }

            page->levels.clear();
            page->category = NULL;
            free((char*)page->name);
            free((char*)page->description);
            delete page;
        }

        category->pages.clear();
        free((char*)category->name);
        free((char*)category->description);
        delete category;
    }
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
        } else if (type.compare("des") == 0) {
            addDescription(data);
        } else {
            CCLog("LevelLoader: Invalid type: %s", line.c_str());
        }
    }
}

void LevelLoader::addCategory(const std::string& data)
{
    currentCategory = new LoaderCategory();
    currentCategory->uid = ++idCounter;
    currentCategory->localid = ++categoryLocalCounter;
    currentCategory->prev = NULL;
    currentCategory->next = NULL;
    currentCategory->description = NULL;
    currentCategory->name = strdup(data.c_str());

    if (categories.size() > 0) {
        auto lastCategory = categories.back();
        currentCategory->prev = lastCategory;
        lastCategory->next = currentCategory;
    }
    
    categories.push_back(currentCategory);
    lastRecord = currentCategory;
    pageLocalCounter = 0;
}

void LevelLoader::addPage(const std::string& data)
{
    assert(currentCategory && "category record must come first");

    currentPage = new LoaderPage();
    currentPage->uid = ++idCounter;
    currentPage->localid = ++pageLocalCounter;
    currentPage->prev = NULL;
    currentPage->next = NULL;
    currentPage->description = NULL;
    currentPage->name = strdup(data.c_str());
    currentPage->category = currentCategory;

    if (currentCategory->pages.size() > 0) {
        auto lastPage = currentCategory->pages.back();
        currentPage->prev = lastPage;
        lastPage->next = currentPage;
    }

    currentCategory->pages.push_back(currentPage);
    lastRecord = currentPage;
    levelLocalCounter = 0;
}

void LevelLoader::addLevel(const std::string& data)
{
    assert(currentPage && "page record must come first");

    LoaderLevel* currentLevel = new LoaderLevel();
    currentLevel->uid = ++idCounter;
    currentLevel->prev = NULL;
    currentLevel->next = NULL;
    currentLevel->description = NULL;
    currentLevel->localid = ++levelLocalCounter;
    currentLevel->data = strdup(data.c_str());
    currentLevel->page = currentPage;

    if (currentPage->levels.size() > 0) {
        auto lastLevel = currentPage->levels.back();
        currentLevel->prev = lastLevel;
        lastLevel->next = currentLevel;
    }
    
    currentPage->levels.push_back(currentLevel);
    lastRecord = currentLevel;
}

void LevelLoader::addDescription(const std::string& data)
{
    assert(lastRecord && "one record must come first");

    lastRecord->description = strdup(data.c_str());
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

unsigned int LoaderCategory::countLevels()
{
    unsigned int sum = 0;
    for (auto page : pages) {
        sum += page->levels.size();
    }
    return sum;
}

unsigned int LoaderCategory::countLevelsSolved()
{
    return userstate::getStarsForCategory(this);
}

const char* LoaderCategory::getLocalizedName() const
{
    static char key[10] = {0};
    snprintf(key, sizeof(key), "%d.name", localid);
    return _("level.category", key).get().c_str();
}

const char* LoaderCategory::getLocalizedDescription() const
{
    static char key[10] = {0};
    snprintf(key, sizeof(key), "%d.desc", localid);
    return _("level.category", key).get().c_str();
}

const char* LoaderPage::getLocalizedName() const
{
    static char key[10] = {0};
    snprintf(
        key, sizeof(key),
        "%d.%d",
        category->localid,
        localid
    );

    static char result[100] = {0};
    snprintf(
        result, sizeof(result),
        _("level.pages", key).get().c_str(),
        category->getLocalizedName()
    );

    return result;
}

const char* LoaderLevel::getLocalizedName() const
{
    static char result[100] = {0};
    snprintf(
        result, sizeof(result),
        "%s #%d",
        page->getLocalizedName(),
        localid
    );

    return result;
}
