#define RAYGUI_IMPLEMENTATION
#include "Renderer.hpp"
#include "Question.hpp"
#include "QuestionSign.hpp"

#include <string>

namespace noonoo {

Renderer::Renderer(int width, int height)
  :_width(width), _height(height)
{
  InitWindow(width, height, "noonoo");
  SetTargetFPS(60);
}

Renderer::~Renderer()
{
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
    case QuestionSign::SIGN_MUL:   sign = "*"; break;
    case QuestionSign::SIGN_DIV:   sign = "/"; break;
  }
  std::string text =
    std::to_string((int)q->GetLValue()) + " " +
    sign + " " +
    std::to_string((int)q->GetRValue()) + " = ?";
  DrawText(text.c_str(), _width / 2 - 50, _height / 2 - 10, 20, BLACK);
}

} // namespace noonoo
