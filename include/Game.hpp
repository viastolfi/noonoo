#ifndef GAME_HPP
#define GAME_HPP

#include "Difficulty.hpp"
#include "Question.hpp"
#include "Renderer.hpp"
#include <memory>

namespace noonoo {

enum class GameState { SelectingDifficulty, Asking, ShowingResult, GameOver };

class Game {
public:
  Game(std::unique_ptr<Renderer> r);
  ~Game() = default;
  void Run();
private:
  std::unique_ptr<Renderer> _renderer;
  std::unique_ptr<Question> _current_question;
  GameState _state = GameState::SelectingDifficulty;
  Difficulty _difficulty = Difficulty::Easy;
  bool _was_correct = false;
  float _timeRemaining = 0.0f;
  float _timerDuration = 0.0f;
  int _score = 0;
  int _total = 0;
};

}; // namespace noonoo

#endif // GAME_HPP
