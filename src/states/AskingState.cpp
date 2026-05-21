#include "states/AskingState.hpp"
#include "Game.hpp"
#include "Renderer.hpp"

namespace noonoo {

void AskingState::Update(Game& game, Renderer& renderer)
{
  game.DecrementTime(renderer.GetDeltaTime());
  if (game.GetTimeRemaining() <= 0.0f) {
    game.GoToGameOver();
    return;
  }

  renderer.DrawTimer(game.GetTimeRemaining(), game.GetTimerDuration());
  renderer.DrawQuestion(game.GetCurrentQuestion());

  int clicked = -1;
  renderer.DrawAnswerButtons(game.GetCurrentQuestion()->GetPossibleAnswers(), clicked);

  if (clicked >= 0) {
    double selected = game.GetCurrentQuestion()->GetPossibleAnswers()[clicked];
    game.SubmitAnswer(selected);
    game.GoToShowingResult();
  }
}

} // namespace noonoo
