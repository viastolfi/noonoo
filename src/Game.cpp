#include "Game.hpp"

namespace noonoo {

Game::Game(std::unique_ptr<Renderer> r)
  :_renderer(std::move(r))
{
  _current_question = std::make_unique<Question>(
    10, 10, QuestionSign::SIGN_PLUS
  );
}

void Game::Run()
{
  while (!_renderer->ShouldWindowClose())
  {
    _renderer->BeginDraw();

    if (_current_question) {
      _renderer->DrawQuestion(_current_question.get());
    }

    _renderer->DrawButton((Rectangle) {24,24,120,30}, "#191#Show Message", _showMessageBox);

    if (_showMessageBox)
    {
      _renderer->DrawMessageBox(
          (Rectangle){85,70,250,100}, 
          "#191#Message Box", 
          "Hi! This is a message!", 
          _clicked, "Nice", "Cool");

      if (_clicked >= 0) _showMessageBox = false;
    }

    _renderer->EndDraw();
  }
}

}; // namespace noonoo
