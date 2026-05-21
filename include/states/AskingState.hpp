#ifndef ASKING_STATE_HPP
#define ASKING_STATE_HPP

#include "IGameState.hpp"

namespace noonoo {

class AskingState : public IGameState {
public:
  void Update(Game& game, Renderer& renderer) override;
};

} // namespace noonoo

#endif // ASKING_STATE_HPP
