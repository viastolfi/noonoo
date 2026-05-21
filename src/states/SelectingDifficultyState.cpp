#include "states/SelectingDifficultyState.hpp"
#include "Difficulty.hpp"
#include "Game.hpp"
#include "Renderer.hpp"

namespace noonoo {

void SelectingDifficultyState::Update(Game& game, Renderer& renderer)
{
  Difficulty difficulty = Difficulty::Easy;
  bool chosen = false;
  bool showHistory = false;
  renderer.DrawDifficultyScreen(difficulty, chosen, showHistory);
  if (chosen)
    game.StartGame(difficulty);
  else if (showHistory)
    game.GoToHistory();
}

} // namespace noonoo
