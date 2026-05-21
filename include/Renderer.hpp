#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <cstdarg>
#include <string>

#include "raylib.h"
#include "thirdparty/raygui.h"

namespace noonoo { class Question; }

namespace noonoo {

class Renderer {
public:
  Renderer(int width, int height);
  ~Renderer();

  bool ShouldWindowClose();  
  void BeginDraw();
  void EndDraw();
  void DrawButton(Rectangle bounds, const char* text, bool& result);
  void DrawQuestion(const Question* q);
 
#define DrawMessageBox(bounds, title, message, clicked, first, ...) \
  DrawMessageBoxNull(bounds, title, message, clicked, first, __VA_ARGS__, NULL)
  void DrawMessageBoxNull(
      Rectangle bounds, 
      const char* title, 
      const char* message,
      int& clicked,
      const char* first, ...);
  private:
  int _width;
  int _height;
  bool _shouldWindowClose = false;
};

} // namespace noonoo

#endif // RENDERER_HPP
