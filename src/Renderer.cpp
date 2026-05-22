#define RAYGUI_IMPLEMENTATION
#include "Renderer.hpp"
#include "Mascot.hpp"
#include "Question.hpp"
#include "QuestionSign.hpp"

#include <string>
#include <vector>

namespace noonoo {

static const char* difficultyLabel(Difficulty d)
{
  switch (d) {
    case Difficulty::Easy:   return "Easy";
    case Difficulty::Medium: return "Medium";
    case Difficulty::Hard:   return "Hard";
  }
  return "?";
}

Renderer::Renderer(int width, int height)
  : _width(width), _height(height)
{
  InitWindow(width, height, "noonoo");
  SetTargetFPS(60);
  // load font after window init — raylib requires an active context for texture upload.
  // size 128 ensures sharp rendering at large display sizes.
  _font = LoadFontEx("assets/fonts/Helvetica-Bold.ttf", 128, nullptr, 0);
  GuiSetFont(_font);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 28);
}

Renderer::~Renderer()
{
  UnloadFont(_font);
  UnloadTexture(_mascotTexture);
  CloseWindow();
}

bool Renderer::ShouldWindowClose()
{
  return WindowShouldClose() || _shouldWindowClose;
}

float Renderer::GetDeltaTime() const
{
  return GetFrameTime();
}

float Renderer::GetMouseWheelMove() const
{
  return ::GetMouseWheelMove();
}

void Renderer::BeginDraw()
{
  BeginDrawing();
  ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
}

void Renderer::EndDraw()
{
  EndDrawing();
}

void Renderer::DrawButton(Rectangle bounds, const char* text, bool& result)
{
  if (GuiButton(bounds, text)) result = true;
}

void Renderer::DrawMessageBoxNull(
    Rectangle bounds,
    const char* title,
    const char* message,
    int& clicked,
    const char* first, ...)
{
  std::string buttons = first;

  std::va_list args;
  va_start(args, first);
  const char* b = va_arg(args, const char*);
  while (b != NULL) {
    buttons += ";";
    buttons += b;
    b = va_arg(args, const char*);
  }
  va_end(args);

  clicked = GuiMessageBox(bounds, title, message, buttons.c_str());
}

void Renderer::DrawQuestion(const Question* q)
{
  std::string text = q->ToString();
  float fontSize = 36.0f;
  float textWidth = MeasureTextEx(_font, text.c_str(), fontSize, 1).x;
  float y = _height * 0.35f;
  DrawTextEx(_font, text.c_str(), { (_width - textWidth) / 2.0f, y }, fontSize, 1, BLACK);
}

void Renderer::DrawAnswerButtons(const std::vector<double>& answers, int& clicked)
{
  clicked = -1;
  const float btnWidth  = 160.0f;
  const float btnHeight = 50.0f;
  const float gap       = 30.0f;
  const float totalWidth = answers.size() * btnWidth + (answers.size() - 1) * gap;
  const float startX = (_width - totalWidth) / 2.0f;
  const float y = _height * 0.52f;

  for (size_t i = 0; i < answers.size(); i++) {
    Rectangle bounds = {
      startX + (float)i * (btnWidth + gap),
      y,
      btnWidth,
      btnHeight
    };
    std::string label = std::to_string((int)answers[i]);
    bool pressed = false;
    DrawButton(bounds, label.c_str(), pressed);
    if (pressed) clicked = (int)i;
  }
}

void Renderer::DrawFeedback(bool correct)
{
  const char* text = correct ? "Correct!" : "Wrong!";
  Color color = correct ? Color{0, 160, 60, 255} : Color{200, 50, 50, 255};
  float fontSize = 38.0f;
  float textWidth = MeasureTextEx(_font, text, fontSize, 1).x;
  float y = _height * 0.43f;
  DrawTextEx(_font, text, { (_width - textWidth) / 2.0f, y }, fontSize, 1, color);
}

void Renderer::DrawNextButton(bool& next)
{
  next = false;
  const float btnWidth = 180.0f, btnHeight = 50.0f;
  DrawButton(
    { (_width - btnWidth) / 2.0f, _height * 0.65f, btnWidth, btnHeight },
    "Next",
    next
  );
}

void Renderer::DrawTimer(float secondsRemaining, float totalDuration)
{
  float ratio = totalDuration > 0.0f ? secondsRemaining / totalDuration : 0.0f;
  if (ratio < 0.0f) ratio = 0.0f;
  if (ratio > 1.0f) ratio = 1.0f;

  // two-segment lerp: green -> yellow -> red as time runs out
  Color green  = { 0,   200, 80,  255 };
  Color yellow = { 230, 190, 0,   255 };
  Color red    = { 220, 50,  50,  255 };

  Color barColor;
  if (ratio > 0.5f) {
    float t = (ratio - 0.5f) * 2.0f;
    barColor = {
      (unsigned char)(yellow.r + t * (green.r  - yellow.r)),
      (unsigned char)(yellow.g + t * (green.g  - yellow.g)),
      (unsigned char)(yellow.b + t * (green.b  - yellow.b)),
      255
    };
  } else {
    float t = ratio * 2.0f;
    barColor = {
      (unsigned char)(red.r + t * (yellow.r - red.r)),
      (unsigned char)(red.g + t * (yellow.g - red.g)),
      (unsigned char)(red.b + t * (yellow.b - red.b)),
      255
    };
  }

  // badge occupies top-left corner (cx=44, r=22), so start after it
  const float barX = 80, barY = 12, barHeight = 20;
  const float gap = 10, rightMargin = 15;
  // reserve space for the widest possible label so the bar width stays stable
  float textWidth = MeasureTextEx(_font, "90s", 20, 1).x;
  float barWidth = _width - barX - gap - textWidth - rightMargin;

  DrawRectangle((int)barX, (int)barY, (int)barWidth, (int)barHeight, LIGHTGRAY);
  DrawRectangle((int)barX, (int)barY, (int)(barWidth * ratio), (int)barHeight, barColor);
  DrawRectangleLinesEx({ barX, barY, barWidth, barHeight }, 1, DARKGRAY);

  int seconds = (int)secondsRemaining + 1;
  if (seconds < 0) seconds = 0;
  std::string text = std::to_string(seconds) + "s";
  DrawTextEx(_font, text.c_str(), { barX + barWidth + gap, barY - 1.0f }, 20, 1, DARKGRAY);
}

void Renderer::DrawDifficultyScreen(Difficulty& selected, bool& chosen, bool& showHistory)
{
  chosen = false;
  showHistory = false;

  const float btnWidth = 220.0f, btnHeight = 50.0f, gap = 18.0f;
  // 3 difficulty buttons + extra gap + history button
  const float totalH = 3 * (btnHeight + gap) + 20.0f + btnHeight;
  const float startY = (_height - totalH) / 2.0f;
  const float btnX   = (_width - btnWidth) / 2.0f;

  const char* title = "Select Difficulty";
  float titleWidth = MeasureTextEx(_font, title, 36, 1).x;
  DrawTextEx(_font, title, { (_width - titleWidth) / 2.0f, startY - 60.0f }, 36, 1, BLACK);

  struct Option { const char* label; Difficulty difficulty; };
  Option options[] = {
    { "Easy",   Difficulty::Easy   },
    { "Medium", Difficulty::Medium },
    { "Hard",   Difficulty::Hard   }
  };

  for (int i = 0; i < 3; i++) {
    bool pressed = false;
    DrawButton({ btnX, startY + i * (btnHeight + gap), btnWidth, btnHeight }, options[i].label, pressed);
    if (pressed) {
      selected = options[i].difficulty;
      chosen = true;
    }
  }

  bool histPressed = false;
  DrawButton({ btnX, startY + 3 * (btnHeight + gap) + 20.0f, btnWidth, btnHeight }, "History", histPressed);
  if (histPressed) showHistory = true;
}

void Renderer::DrawGameOver(int score, int total, bool& playAgain, bool leveledUp, int level)
{
  playAgain = false;

  const char* title = "Time's up!";
  float titleWidth = MeasureTextEx(_font, title, 40, 1).x;
  DrawTextEx(_font, title, { (_width - titleWidth) / 2.0f, _height * 0.22f }, 40, 1, BLACK);

  std::string scoreText = "Score: " + std::to_string(score) + " / " + std::to_string(total);
  float scoreWidth = MeasureTextEx(_font, scoreText.c_str(), 28, 1).x;
  DrawTextEx(_font, scoreText.c_str(), { (_width - scoreWidth) / 2.0f, _height * 0.36f }, 28, 1, DARKGRAY);

  if (leveledUp) {
    const char* lvlText = TextFormat("Level Up!  Lv.%d", level);
    float lvlW = MeasureTextEx(_font, lvlText, 32, 1).x;
    Color gold = { 255, 180, 0, 255 };
    DrawTextEx(_font, lvlText, { (_width - lvlW) / 2.0f, _height * 0.50f }, 32, 1, gold);
  }

  const float btnWidth = 200.0f, btnHeight = 50.0f;
  bool pressed = false;
  DrawButton({ (_width - btnWidth) / 2.0f, _height * 0.66f, btnWidth, btnHeight }, "Play Again", pressed);
  if (pressed) playAgain = true;
}

void Renderer::DrawSessionListScreen(const std::vector<GameSession>& sessions, int& clickedIndex, bool& back)
{
  clickedIndex = -1;
  back = false;

  const char* title = "History";
  float titleWidth = MeasureTextEx(_font, title, 36, 1).x;
  DrawTextEx(_font, title, { (_width - titleWidth) / 2.0f, 20.0f }, 36, 1, BLACK);

  if (sessions.empty()) {
    const char* empty = "No games played yet.";
    float w = MeasureTextEx(_font, empty, 22, 1).x;
    DrawTextEx(_font, empty, { (_width - w) / 2.0f, _height * 0.4f }, 22, 1, DARKGRAY);
  } else {
    const float rowH    = 54.0f;
    const float startY  = 76.0f;
    const float rowW    = _width - 80.0f;
    const float rowX    = 40.0f;

    int maxVisible = (int)((_height - startY - 60.0f) / rowH);
    int first = (int)sessions.size() - maxVisible;
    if (first < 0) first = 0;

    // show most recent first
    int row = 0;
    for (int i = (int)sessions.size() - 1; i >= first; i--, row++) {
      const GameSession& s = sessions[i];
      float y = startY + row * rowH;

      std::string label =
        std::string("Game ") + std::to_string(i + 1) + " - " +
        difficultyLabel(s.difficulty) + "  |  " +
        std::to_string(s.score) + "/" + std::to_string(s.total) + " correct";

      bool pressed = false;
      DrawButton({ rowX, y, rowW, rowH - 6.0f }, label.c_str(), pressed);
      if (pressed) clickedIndex = i;
    }
  }

  const float btnWidth = 180.0f, btnHeight = 46.0f;
  bool pressed = false;
  DrawButton({ (_width - btnWidth) / 2.0f, (float)_height - 60.0f, btnWidth, btnHeight }, "Back", pressed);
  if (pressed) back = true;
}

void Renderer::DrawSessionDetailScreen(const GameSession& session, float scrollOffset, bool& back)
{
  back = false;

  // header
  std::string title =
    std::string("Game - ") + difficultyLabel(session.difficulty) +
    "  |  " + std::to_string(session.score) + "/" + std::to_string(session.total) + " correct";
  float titleWidth = MeasureTextEx(_font, title.c_str(), 28, 1).x;
  DrawTextEx(_font, title.c_str(), { (_width - titleWidth) / 2.0f, 18.0f }, 28, 1, BLACK);

  // scrollable list area
  const float listX    = 40.0f;
  const float listY    = 66.0f;
  const float listW    = (float)_width - 80.0f;
  const float listH    = (float)_height - listY - 70.0f;
  const float lineH    = 30.0f;
  const float fontSize = 20.0f;

  BeginScissorMode((int)listX, (int)listY, (int)listW, (int)listH);

  for (size_t i = 0; i < session.questions.size(); i++) {
    const QuestionRecord& q = session.questions[i];
    float y = listY + (float)i * lineH - scrollOffset;
    if (y + lineH < listY || y > listY + listH) continue;  // skip off-screen rows

    std::string ans = std::to_string(q.userAnswer);
    if (!q.wasCorrect) ans += "  (expected " + std::to_string(q.correctAnswer) + ")";
    std::string line = q.questionText + "   ->   " + ans;

    Color color = q.wasCorrect ? Color{0, 150, 60, 255} : Color{200, 50, 50, 255};
    DrawTextEx(_font, line.c_str(), { listX + 8.0f, y }, fontSize, 1, color);
  }

  EndScissorMode();

  // scrollbar hint: thin track on the right
  if (session.questions.size() * lineH > listH) {
    float totalH  = session.questions.size() * lineH;
    float thumbH  = listH * (listH / totalH);
    float thumbY  = listY + (scrollOffset / (totalH - listH)) * (listH - thumbH);
    DrawRectangle((int)(_width - 16), (int)listY, 4, (int)listH, LIGHTGRAY);
    DrawRectangle((int)(_width - 16), (int)thumbY, 4, (int)thumbH, DARKGRAY);
  }

  const float btnWidth = 180.0f, btnHeight = 46.0f;
  bool pressed = false;
  DrawButton({ (_width - btnWidth) / 2.0f, (float)_height - 58.0f, btnWidth, btnHeight }, "Back", pressed);
  if (pressed) back = true;
}

void Renderer::DrawMascot(const Mascot& mascot)
{
    if (!mascot.IsVisible()) return;

    static constexpr float FRAME_SIZE = 16.0f;

    // columns = directions, rows = animation frames
    // col 0 = down, col 1 = up, col 2 = left, col 3 = right
    float srcX;
    switch (mascot.GetDirection()) {
        case MascotDirection::Down:  srcX =  0.0f; break;
        case MascotDirection::Up:    srcX = 16.0f; break;
        case MascotDirection::Left:  srcX = 32.0f; break;
        case MascotDirection::Right: srcX = 48.0f; break;
        default:                     srcX =  0.0f; break;
    }

    float srcY = mascot.GetFrameIndex() * FRAME_SIZE;

    Rectangle src  = { srcX, srcY, FRAME_SIZE, FRAME_SIZE };
    Rectangle dest = { mascot.GetX(), mascot.GetY(), Mascot::DISPLAY_SIZE, Mascot::DISPLAY_SIZE };
    DrawTexturePro(_mascotTexture, src, dest, { 0, 0 }, 0.0f, WHITE);
}

void Renderer::DrawMascotToggle(bool isVisible, bool& clicked)
{
    clicked = false;
    const float btnW = 200.0f, btnH = 34.0f;
    DrawButton(
        { (float)_width - btnW - 10.0f, (float)_height - btnH - 10.0f, btnW, btnH },
        isVisible ? "Mascot ON" : "Mascot OFF",
        clicked
    );
}

void Renderer::SetMascotLevel(int level)
{
    if (level == _mascotLevel) return;
    _mascotLevel = level;

    if (_mascotTexture.id != 0)
        UnloadTexture(_mascotTexture);

    auto config = PlayerLevel::ConfigFor(level);
    std::string path = std::string("assets/sprites/") + config.ninjaFolder + "/Walk.png";
    _mascotTexture = LoadTexture(path.c_str());
}

void Renderer::DrawBadge(int totalCorrect)
{
    // color matches the ninja sprite for each level
    static constexpr Color LEVEL_COLORS[] = {
        {   0, 180,  48, 255 },  // level 1 — green
        {   0, 121, 241, 255 },  // level 2 — blue
        { 230,  41,  55, 255 },  // level 3 — red
        { 255, 200,   0, 255 },  // level 4 — gold
    };

    auto   config = PlayerLevel::ConfigFor(_mascotLevel);
    Color  color  = LEVEL_COLORS[_mascotLevel - 1];

    const float cx = 44.0f;
    const float cy = 44.0f;
    const float r  = 22.0f;

    switch (config.badge) {
        case BadgeShape::Circle:
            DrawCircle((int)cx, (int)cy, (int)r, color);
            DrawCircleLines((int)cx, (int)cy, (int)r, WHITE);
            break;
        case BadgeShape::Triangle:
            DrawTriangle({ cx, cy - r }, { cx - r, cy + r }, { cx + r, cy + r }, color);
            DrawTriangleLines({ cx, cy - r }, { cx - r, cy + r }, { cx + r, cy + r }, WHITE);
            break;
        case BadgeShape::Diamond:
            DrawPoly({ cx, cy }, 4, r, 45.0f, color);
            DrawPolyLinesEx({ cx, cy }, 4, r, 45.0f, 2.0f, WHITE);
            break;
        case BadgeShape::Pentagon:
            DrawPoly({ cx, cy }, 5, r, -90.0f, color);
            DrawPolyLinesEx({ cx, cy }, 5, r, -90.0f, 2.0f, WHITE);
            break;
    }

    DrawTextEx(_font, TextFormat("Lv.%d", _mascotLevel),
        { cx - 18.0f, cy + r + 4.0f }, 18.0f, 1.0f, WHITE);

    // progress bar toward next level
    const float barX = 8.0f, barY = cy + r + 26.0f;
    const float barW = 72.0f, barH = 6.0f;

    if (_mascotLevel < PlayerLevel::MAX_LEVEL) {
        LevelConfig current = PlayerLevel::ConfigFor(_mascotLevel);
        LevelConfig next    = PlayerLevel::ConfigFor(_mascotLevel + 1);
        int   range    = next.threshold - current.threshold;
        int   progress = totalCorrect - current.threshold;
        float ratio    = (range > 0) ? (float)progress / (float)range : 1.0f;
        if (ratio < 0.0f) ratio = 0.0f;
        if (ratio > 1.0f) ratio = 1.0f;

        DrawRectangle((int)barX, (int)barY, (int)barW, (int)barH, { 80, 80, 80, 180 });
        DrawRectangle((int)barX, (int)barY, (int)(barW * ratio), (int)barH, color);
        DrawRectangleLinesEx({ barX, barY, barW, barH }, 1.0f, { 200, 200, 200, 180 });

        DrawTextEx(_font, TextFormat("%d/%d", totalCorrect - current.threshold, range),
            { barX, barY + barH + 2.0f }, 14.0f, 1.0f, { 200, 200, 200, 200 });
    } else {
        // max level — show a full gold bar
        Color gold = { 255, 200, 0, 255 };
        DrawRectangle((int)barX, (int)barY, (int)barW, (int)barH, gold);
        DrawRectangleLinesEx({ barX, barY, barW, barH }, 1.0f, WHITE);
        DrawTextEx(_font, "MAX", { barX + 22.0f, barY + barH + 2.0f }, 14.0f, 1.0f, gold);
    }
}

} // namespace noonoo
