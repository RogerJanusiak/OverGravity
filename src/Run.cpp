#include "../includes/Run.h"

Run::Run(GlobalGameState& ggs) : ggs(ggs), playerEntity(ggs.renderer,10), player(&playerEntity, ggs) {

}
