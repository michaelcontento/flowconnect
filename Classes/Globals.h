#ifndef FlowConnect_Globals_h
#define FlowConnect_Globals_h

#include "LevelLoader.h"
#include "StarButton.h"

#define DEFAULT_FONT_NAME "RobotoCondensed-Bold"
#define SMALL_FONT_NAME "RobotoCondensed-Regular"

extern LevelLoader globalLevelLoader;
extern const LoaderLevel* globalLevel;
extern StarButton* globalLastStarButton;

#endif