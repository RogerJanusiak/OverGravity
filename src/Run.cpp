#include "../includes/Run.h"

Run::Run(GlobalGameState& ggs) : ggs(ggs), m_playerEntity(ggs.renderer,10), m_player(&m_playerEntity, ggs) {

}
