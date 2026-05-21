#include "states/SessionDetailState.hpp"
#include "Game.hpp"
#include "Renderer.hpp"

namespace noonoo {

void SessionDetailState::Update(Game& game, Renderer& renderer)
{
  const auto& sessions = game.GetHistory().GetAllSessions();
  int idx = game.GetSelectedSessionIndex();
  if (idx < 0 || idx >= (int)sessions.size()) {
    game.GoToHistory();
    return;
  }

  const GameSession& session = sessions[idx];
  const float lineHeight = 30.0f;
  const float listAreaHeight = renderer.GetHeight() - 100.0f;
  const float contentHeight = session.questions.size() * lineHeight;
  const float maxScroll = contentHeight > listAreaHeight ? contentHeight - listAreaHeight : 0.0f;

  _scrollOffset -= renderer.GetMouseWheelMove() * 40.0f;
  if (_scrollOffset < 0.0f)       _scrollOffset = 0.0f;
  if (_scrollOffset > maxScroll)  _scrollOffset = maxScroll;

  bool back = false;
  renderer.DrawSessionDetailScreen(session, _scrollOffset, back);
  if (back)
    game.GoToHistory();
}

} // namespace noonoo
