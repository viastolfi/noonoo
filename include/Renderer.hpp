#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <cstdarg>
#include <string>
#include <vector>

#include "Difficulty.hpp"
#include "GameSession.hpp"
#include "Mascot.hpp"
#include "PlayerLevel.hpp"
#include "QuestionRecord.hpp"
#include "raylib.h"
#include "thirdparty/raygui.h"

namespace noonoo { class Question; }

namespace noonoo {

class Renderer {
public:
  Renderer(int width, int height);
  ~Renderer();

  bool  ShouldWindowClose();
  float GetDeltaTime() const;
  float GetMouseWheelMove() const;
  int   GetWidth()  const { return _width; }
  int   GetHeight() const { return _height; }

  void BeginDraw();
  void EndDraw();
  void DrawButton(Rectangle bounds, const char* text, bool& result);
  void DrawQuestion(const Question* q);
  void DrawAnswerButtons(const std::vector<double>& answers, int& clicked);
  void DrawFeedback(bool correct);
  void DrawNextButton(bool& next);
  void DrawTimer(float secondsRemaining, float totalDuration);
  void DrawDifficultyScreen(Difficulty& selected, bool& chosen, bool& showHistory);
  void DrawGameOver(int score, int total, bool& playAgain, bool leveledUp, int level);
  void DrawMascot(const Mascot& mascot);
  void DrawMascotToggle(bool isVisible, bool& clicked);
  void DrawBadge(int totalCorrect);
  void SetMascotLevel(int level);
  void DrawSessionListScreen(const std::vector<GameSession>& sessions, int& clickedIndex, bool& back);
  void DrawSessionDetailScreen(const GameSession& session, float scrollOffset, bool& back);

#define DrawMessageBox(bounds, title, message, clicked, first, ...) \
  DrawMessageBoxNull(bounds, title, message, clicked, first, __VA_ARGS__, NULL)
  void DrawMessageBoxNull(
      Rectangle bounds,
      const char* title,
      const char* message,
      int& clicked,
      const char* first, ...);

private:
  int  _width;
  int  _height;
  bool _shouldWindowClose = false;
  Font      _font;
  Texture2D _mascotTexture = {};
  int       _mascotLevel   = 0;
};

} // namespace noonoo

#endif // RENDERER_HPP
