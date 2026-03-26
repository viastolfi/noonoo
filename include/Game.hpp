#ifndef GAME_HPP
#define GAME_HPP

#include "Renderer.hpp"
#include <memory>

namespace noonoo {

class Game {
public:
  Game(std::unique_ptr<Renderer> r);
  ~Game() = default;
  void Run();
private:
  std::unique_ptr<Renderer> _renderer;
};

}; // namespace noonoo

#endif // GAME_HPP
