#ifndef GAME_HPP
#define GAME_HPP

#include "Renderer.hpp"
#include "Question.hpp"
#include <memory>

namespace noonoo {

enum class GameState { Asking, ShowingResult };

class Game {
public:
  Game(std::unique_ptr<Renderer> r);
  ~Game() = default;
  void Run();
private:
  std::unique_ptr<Renderer> _renderer;
  std::unique_ptr<Question> _current_question;
  GameState _state = GameState::Asking;
  bool _was_correct = false;
};

}; // namespace noonoo

#endif // GAME_HPP
