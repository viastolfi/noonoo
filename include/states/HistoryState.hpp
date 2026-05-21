#ifndef HISTORY_STATE_HPP
#define HISTORY_STATE_HPP

#include "IGameState.hpp"

namespace noonoo {

class HistoryState : public IGameState {
public:
  void Update(Game& game, Renderer& renderer) override;
};

} // namespace noonoo

#endif // HISTORY_STATE_HPP
