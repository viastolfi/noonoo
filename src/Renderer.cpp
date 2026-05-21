#define RAYGUI_IMPLEMENTATION
#include "Renderer.hpp"
#include "Question.hpp"
#include "QuestionSign.hpp"

#include <string>
#include <vector>

namespace noonoo {

Renderer::Renderer(int width, int height)
  :_width(width), _height(height)
{
  InitWindow(width, height, "noonoo");
  SetTargetFPS(60);
  // load font after window init — raylib requires an active context for texture upload.
  _font = LoadFontEx("assets/fonts/Helvetica-Bold.ttf", 64, nullptr, 0);
  GuiSetFont(_font);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
}

Renderer::~Renderer()
{
  UnloadFont(_font);
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
  const char* sign = "?";
  switch (q->GetSign()) {
    case QuestionSign::SIGN_PLUS:  sign = "+"; break;
    case QuestionSign::SIGN_MINUS: sign = "-"; break;
    case QuestionSign::SIGN_MUL:   sign = "x"; break;
    case QuestionSign::SIGN_DIV:   sign = "/"; break;
  }
  std::string text =
    std::to_string((int)q->GetLValue()) + " " +
    sign + " " +
    std::to_string((int)q->GetRValue()) + " = ?";
  float textWidth = MeasureTextEx(_font, text.c_str(), 24, 1).x;
  DrawTextEx(_font, text.c_str(), { (_width - textWidth) / 2.0f, 40.0f }, 24, 1, BLACK);
}

void Renderer::DrawAnswerButtons(const std::vector<double>& answers, int& clicked)
{
  clicked = -1;
  const float buttonWidth = 100;
  const float buttonHeight = 30;
  const float gap = 20;
  const float totalWidth = answers.size() * buttonWidth + (answers.size() - 1) * gap;
  const float startX = (_width - totalWidth) / 2.0f;
  const float y = 120;

  for (size_t i = 0; i < answers.size(); i++) {
    Rectangle bounds = {
      startX + (float)i * (buttonWidth + gap),
      y,
      buttonWidth,
      buttonHeight
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
  Color color = correct ? GREEN : RED;
  float textWidth = MeasureTextEx(_font, text, 28, 1).x;
  DrawTextEx(_font, text, { (_width - textWidth) / 2.0f, 85.0f }, 28, 1, color);
}

void Renderer::DrawDifficultyScreen(Difficulty& selected, bool& chosen){
  chosen = false;

  const char* title = "Select Difficulty";
  float titleWidth = MeasureTextEx(_font, title, 24, 1).x;
  DrawTextEx(_font, title, { (_width - titleWidth) / 2.0f, 25.0f }, 24, 1, BLACK);

  struct Option { const char* label; Difficulty difficulty; };
  Option options[] = {
    { "Easy",   Difficulty::Easy   },
    { "Medium", Difficulty::Medium },
    { "Hard",   Difficulty::Hard   }
  };

  const float btnWidth = 150, btnHeight = 30, startY = 70, gap = 10;
  const float btnX = (_width - btnWidth) / 2.0f;

  for (int i = 0; i < 3; i++) {
    bool pressed = false;
    DrawButton({ btnX, startY + i * (btnHeight + gap), btnWidth, btnHeight }, options[i].label, pressed);
    if (pressed) {
      selected = options[i].difficulty;
      chosen = true;
    }
  }
}

void Renderer::DrawTimer(float secondsRemaining, float totalDuration)
{
  float ratio = totalDuration > 0.0f ? secondsRemaining / totalDuration : 0.0f;
  if (ratio < 0.0f) ratio = 0.0f;
  if (ratio > 1.0f) ratio = 1.0f;

  // two-segment lerp: green → yellow → red as time runs out
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

  const float barX = 10, barY = 8, barHeight = 16;
  const float gap = 8, rightMargin = 10;
  // reserve space for the widest possible label so the bar width stays stable
  float textWidth = MeasureTextEx(_font, "90s", 18, 1).x;
  float barWidth = _width - barX - gap - textWidth - rightMargin;

  DrawRectangle((int)barX, (int)barY, (int)barWidth, (int)barHeight, LIGHTGRAY);
  DrawRectangle((int)barX, (int)barY, (int)(barWidth * ratio), (int)barHeight, barColor);
  DrawRectangleLinesEx({ barX, barY, barWidth, barHeight }, 1, DARKGRAY);

  int seconds = (int)secondsRemaining + 1;
  if (seconds < 0) seconds = 0;
  std::string text = std::to_string(seconds) + "s";
  DrawTextEx(_font, text.c_str(), { barX + barWidth + 8.0f, barY - 1.0f }, 18, 1, DARKGRAY);
}

void Renderer::DrawGameOver(int score, int total, bool& playAgain)
{
  playAgain = false;

  const char* title = "Time's up!";
  float titleWidth = MeasureTextEx(_font, title, 28, 1).x;
  DrawTextEx(_font, title, { (_width - titleWidth) / 2.0f, 25.0f }, 28, 1, BLACK);

  std::string scoreText = "Score: " + std::to_string(score) + " / " + std::to_string(total);
  float scoreWidth = MeasureTextEx(_font, scoreText.c_str(), 22, 1).x;
  DrawTextEx(_font, scoreText.c_str(), { (_width - scoreWidth) / 2.0f, 75.0f }, 22, 1, DARKGRAY);

  const float btnWidth = 150, btnHeight = 30;
  bool pressed = false;
  DrawButton({ (_width - btnWidth) / 2.0f, 125.0f, btnWidth, btnHeight }, "Play Again", pressed);
  if (pressed) playAgain = true;
}

} // namespace noonoo
