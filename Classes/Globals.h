#ifndef FlowConnect_Globals_h
#define FlowConnect_Globals_h

#include "LevelLoader.h"
#include "StarButton.h"

#define DEFAULT_FONT_NAME "RobotoCondensed-Bold"
#define SMALL_FONT_NAME "RobotoCondensed-Regular"

#define PRICE_REMOVE_ADS 50
#define PRICE_HINT 1
#define PRICE_PAGE 30

extern LevelLoader globalLevelLoader;
extern const LoaderLevel* globalLevel;
extern StarButton* globalLastStarButton;

#endif