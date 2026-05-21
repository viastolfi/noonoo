#include "Renderer.hpp"
#include "Game.hpp"

#include <memory>

int main()
{
  std::unique_ptr<noonoo::Renderer> r = std::make_unique<noonoo::Renderer>(1152, 648);
  noonoo::Game g(std::move(r));
  g.Run();
  return 0;
}
