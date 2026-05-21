#include "Game.hpp"
#include "QuestionFactory.hpp"

namespace noonoo {

// timer duration in seconds per difficulty level
static float timerDuration(Difficulty d)
{
  switch (d) {
    case Difficulty::Easy:   return 90.0f;
    case Difficulty::Medium: return 60.0f;
    case Difficulty::Hard:   return 45.0f;
  }
  return 60.0f;
}

Game::Game(std::unique_ptr<Renderer> r)
  : _renderer(std::move(r))
{
}

void Game::Run()
{
  while (!_renderer->ShouldWindowClose())
  {
    _renderer->BeginDraw();

    if (_state == GameState::SelectingDifficulty)
    {
      bool chosen = false;
      _renderer->DrawDifficultyScreen(_difficulty, chosen);
      if (chosen)
      {
        _timeRemaining = timerDuration(_difficulty);
        _timerDuration = _timeRemaining;
        _score = 0;
        _total = 0;
        _current_question = QuestionFactory::Create(_difficulty);
        _state = GameState::Asking;
      }
    }
    else if (_state == GameState::Asking)
    {
      _timeRemaining -= _renderer->GetDeltaTime();
      if (_timeRemaining <= 0.0f)
      {
        _state = GameState::GameOver;
      }
      else
      {
        _renderer->DrawTimer(_timeRemaining, _timerDuration);
        _renderer->DrawQuestion(_current_question.get());

        int clicked = -1;
        _renderer->DrawAnswerButtons(_current_question->GetPossibleAnswers(), clicked);

        if (clicked >= 0)
        {
          double selected = _current_question->GetPossibleAnswers()[clicked];
          _was_correct = ((int)selected == (int)_current_question->GetResult());
          if (_was_correct) _score++;
          _total++;
          _state = GameState::ShowingResult;
        }
      }
    }
    else if (_state == GameState::ShowingResult)
    {
      _timeRemaining -= _renderer->GetDeltaTime();
      if (_timeRemaining <= 0.0f)
      {
        _state = GameState::GameOver;
      }
      else
      {
        _renderer->DrawTimer(_timeRemaining, _timerDuration);
        _renderer->DrawQuestion(_current_question.get());
        _renderer->DrawFeedback(_was_correct);

        bool next = false;
        _renderer->DrawButton({150, 160, 100, 30}, "Next", next);

        if (next)
        {
          _current_question = QuestionFactory::Create(_difficulty);
          _state = GameState::Asking;
        }
      }
    }
    else
    {
      bool playAgain = false;
      _renderer->DrawGameOver(_score, _total, playAgain);
      if (playAgain)
        _state = GameState::SelectingDifficulty;
    }

    _renderer->EndDraw();
  }
}

}; // namespace noonoo
