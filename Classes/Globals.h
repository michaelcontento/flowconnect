#ifndef FlowConnect_Globals_h
#define FlowConnect_Globals_h

#include "LevelLoader.h"

LevelLoader globalLevelLoader = LevelLoader("levels");
const LoaderLevel* globalLevel = globalLevelLoader.getCategories().front()->pages.front()->levels.front();

#endif