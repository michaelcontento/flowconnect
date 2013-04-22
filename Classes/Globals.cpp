#include "Globals.h"

LevelLoader globalLevelLoader = LevelLoader("levels");
const LoaderLevel* globalLevel = globalLevelLoader.getCategories().front()->pages.front()->levels.front();
StarButton* globalLastStarButton = NULL;