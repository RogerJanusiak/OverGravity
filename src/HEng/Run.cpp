#include "../../includes/HEng/Run.h"

Run::Run(GlobalGameState& ggs, const int level) : ggs(ggs), playerEntity(ggs.renderer,10), player(&playerEntity, ggs), currentLevel(Level(ggs,level)) {}
