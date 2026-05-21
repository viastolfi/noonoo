#include "Game.hpp"
#include "QuestionFactory.hpp"

namespace noonoo {

Game::Game(std::unique_ptr<Renderer> r)
  : _renderer(std::move(r))
{
  _current_question = QuestionFactory::Create();
}

void Game::Run()
{
  while (!_renderer->ShouldWindowClose())
  {
    _renderer->BeginDraw();

    if (_state == GameState::Asking)
    {
      _renderer->DrawQuestion(_current_question.get());

      int clicked = -1;
      _renderer->DrawAnswerButtons(_current_question->GetPossibleAnswers(), clicked);

      if (clicked >= 0)
      {
        double selected = _current_question->GetPossibleAnswers()[clicked];
        _was_correct = ((int)selected == (int)_current_question->GetResult());
        _state = GameState::ShowingResult;
      }
    }
    else
    {
      _renderer->DrawQuestion(_current_question.get());
      _renderer->DrawFeedback(_was_correct);

      bool next = false;
      _renderer->DrawButton({150, 160, 100, 30}, "Next", next);

      if (next)
      {
        _current_question = QuestionFactory::Create();
        _state = GameState::Asking;
      }
    }

    _renderer->EndDraw();
  }
}

}; // namespace noonoo
