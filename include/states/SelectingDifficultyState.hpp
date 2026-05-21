#ifndef SELECTING_DIFFICULTY_STATE_HPP
#define SELECTING_DIFFICULTY_STATE_HPP

#include "IGameState.hpp"

namespace noonoo {

class SelectingDifficultyState : public IGameState {
public:
  void Update(Game& game, Renderer& renderer) override;
};

} // namespace noonoo

#endif // SELECTING_DIFFICULTY_STATE_HPP
