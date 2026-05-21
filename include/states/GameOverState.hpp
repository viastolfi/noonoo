#ifndef GAME_OVER_STATE_HPP
#define GAME_OVER_STATE_HPP

#include "IGameState.hpp"

namespace noonoo {

class GameOverState : public IGameState {
public:
  void Update(Game& game, Renderer& renderer) override;
};

} // namespace noonoo

#endif // GAME_OVER_STATE_HPP
