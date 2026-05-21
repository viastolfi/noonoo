#ifndef SESSION_DETAIL_STATE_HPP
#define SESSION_DETAIL_STATE_HPP

#include "IGameState.hpp"

namespace noonoo {

class SessionDetailState : public IGameState {
public:
  void Update(Game& game, Renderer& renderer) override;

private:
  float _scrollOffset = 0.0f;
};

} // namespace noonoo

#endif // SESSION_DETAIL_STATE_HPP
