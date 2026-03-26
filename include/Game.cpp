#include "Game.hpp"

namespace noonoo {

Game::Game(std::unique_ptr<Renderer> r)
  :_renderer(std::move(r))
{}

void Game::Run()
{
  bool showMessageBox = false;
  int clicked = -1;

  while (!r.ShouldWindowClose())
  {
    r.BeginDraw();

    r.DrawButton((Rectangle) {24,24,120,30}, "#191#Show Message", showMessageBox);

    if (showMessageBox)
    {
      r.DrawMessageBox(
          (Rectangle){85,70,250,100}, 
          "#191#Message Box", 
          "Hi! This is a message!", 
          clicked, "Nice", "Cool");

      if (clicked >= 0) showMessageBox = false;
    }

    r.EndDraw();
  }
}

}; // namespace noonoo
