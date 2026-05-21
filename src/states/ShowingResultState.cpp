#include "states/ShowingResultState.hpp"
#include "Game.hpp"
#include "Renderer.hpp"

namespace noonoo {

void ShowingResultState::Update(Game& game, Renderer& renderer)
{
  game.DecrementTime(renderer.GetDeltaTime());
  if (game.GetTimeRemaining() <= 0.0f) {
    game.GoToGameOver();
    return;
  }

  renderer.DrawTimer(game.GetTimeRemaining(), game.GetTimerDuration());
  renderer.DrawQuestion(game.GetCurrentQuestion());
  renderer.DrawFeedback(game.WasLastAnswerCorrect());

  bool next = false;
  renderer.DrawNextButton(next);
  if (next) {
    game.GenerateNextQuestion();
    game.GoToAsking();
  }
}

} // namespace noonoo
