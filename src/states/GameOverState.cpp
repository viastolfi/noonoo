#include "states/GameOverState.hpp"
#include "Game.hpp"
#include "Renderer.hpp"

namespace noonoo {

void GameOverState::Update(Game& game, Renderer& renderer)
{
  bool playAgain = false;
  renderer.DrawGameOver(game.GetScore(), game.GetTotal(), playAgain);
  if (playAgain)
    game.GoToSelectingDifficulty();
}

} // namespace noonoo
