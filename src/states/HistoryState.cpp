#include "states/HistoryState.hpp"
#include "Game.hpp"
#include "Renderer.hpp"

namespace noonoo {

void HistoryState::Update(Game& game, Renderer& renderer)
{
  bool back = false;
  int clicked = -1;
  renderer.DrawSessionListScreen(game.GetHistory().GetAllSessions(), clicked, back);
  if (clicked >= 0)
    game.GoToSessionDetail(clicked);
  else if (back)
    game.GoToSelectingDifficulty();
}

} // namespace noonoo
