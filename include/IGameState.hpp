#ifndef IGAME_STATE_HPP
#define IGAME_STATE_HPP

namespace noonoo {

class Game;
class Renderer;

class IGameState {
public:
  virtual ~IGameState() = default;
  virtual void Update(Game& game, Renderer& renderer) = 0;
};

} // namespace noonoo

#endif // IGAME_STATE_HPP
