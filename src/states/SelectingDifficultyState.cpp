#include "states/SelectingDifficultyState.hpp"
#include "Difficulty.hpp"
#include "Game.hpp"
#include "Renderer.hpp"

namespace noonoo {

void SelectingDifficultyState::Update(Game& game, Renderer& renderer)
{
  Difficulty difficulty = Difficulty::Easy;
  bool chosen = false;
  renderer.DrawDifficultyScreen(difficulty, chosen);
  if (chosen)
    game.StartGame(difficulty);
}

} // namespace noonoo
