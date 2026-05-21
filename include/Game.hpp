#ifndef GAME_HPP
#define GAME_HPP

#include "Renderer.hpp"
#include "Question.hpp"
#include "QuestionSign.hpp"
#include <memory>

namespace noonoo {

class Game {
public:
  Game(std::unique_ptr<Renderer> r);
  ~Game() = default;
  void Run();
private:
  std::unique_ptr<Renderer> _renderer;
  std::unique_ptr<Question> _current_question;
  bool _showMessageBox = false;
  int _clicked = -1;
};

}; // namespace noonoo

#endif // GAME_HPP
