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
  // Load font after window init — raylib requires an active context for texture upload.
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

} // namespace noonoo
