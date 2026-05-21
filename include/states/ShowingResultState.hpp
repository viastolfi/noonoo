#ifndef SHOWING_RESULT_STATE_HPP
#define SHOWING_RESULT_STATE_HPP

#include "IGameState.hpp"

namespace noonoo {

class ShowingResultState : public IGameState {
public:
  void Update(Game& game, Renderer& renderer) override;
};

} // namespace noonoo

#endif // SHOWING_RESULT_STATE_HPP
