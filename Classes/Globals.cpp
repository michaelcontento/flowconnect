#include "Globals.h"

#include "buttons/StarButton.h"

LevelLoader globalLevelLoader;
const LoaderLevel* globalLevel = NULL;
StarButton* globalLastStarButton = NULL;
std::vector<LoaderLevel*> globalAttackLevels;
std::vector<LoaderLevel*>::iterator globalAttackIterator = globalAttackLevels.end();