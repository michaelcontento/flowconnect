#ifndef FlowConnect_Globals_h
#define FlowConnect_Globals_h

#include "LevelLoader.h"

#define DEFAULT_FONT_NAME "RobotoCondensed-Bold"
#define DEFAULT_FNT_42 "RobotoCondensed-Bold-42.fnt"
#define DEFAULT_FNT_36 "RobotoCondensed-Bold-36.fnt"
#define SMALL_FONT_NAME "RobotoCondensed-Regular"
#define SMALL_FNT_36 "RobotoCondensed-Regular-36.fnt"
#define SMALL_FNT_28 "RobotoCondensed-Regular-28.fnt"

#define PRICE_REMOVE_ADS 60
#define PRICE_HINT 1
#define PRICE_PAGE 33

class StarButton;

extern LevelLoader globalLevelLoader;
extern const LoaderLevel* globalLevel;
extern StarButton* globalLastStarButton;

#endif