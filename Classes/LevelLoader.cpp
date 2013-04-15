#include "LevelLoader.h"

using namespace cocos2d;

LevelLoader::LevelLoader(const char* filename)
: categories()
{
    LoaderCategory* category = new LoaderCategory();
    category->uid = 1;
    category->name = (char*)"anfänger";
    category->pages = std::vector<LoaderPage*>();

    LoaderPage* page = new LoaderPage();
    page->uid = 2;
    page->name = (char*)"deppen 4x4";
    page->levels = std::vector<LoaderLevel*>();
    page->category = category;
    category->pages.push_back(page);

    LoaderLevel* level = new LoaderLevel();
    level->uid = 3;
    level->page = page;
    level->size = CCSize(4, 4);
    level->data = (char*)"ll4lldu1l3udrdurur2u";
    page->levels.push_back(level);

    level = new LoaderLevel();
    level->uid = 4;
    level->page = page;
    level->size = CCSize(5, 5);
    level->data = (char*)"3rrrrdudllldrrr4du1rd5dlll2lr6r";
    page->levels.push_back(level);

    categories.push_back(category);
}

LevelLoader::~LevelLoader()
{
    for (auto category : categories) {
        for (auto page : category->pages) {
            for (auto level : page->levels) {
                delete level;
            }
            delete page;
        }
        delete category;
    }
}