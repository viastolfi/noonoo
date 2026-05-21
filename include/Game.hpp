#ifndef GAME_HPP
#define GAME_HPP

#include "Difficulty.hpp"
#include "IGameState.hpp"
#include "IHistoryRepository.hpp"
#include "Question.hpp"
#include "Renderer.hpp"
#include <memory>

namespace noonoo {

class Game {
public:
  Game(std::unique_ptr<Renderer> r);
  ~Game();
  void Run();

  void GoToSelectingDifficulty();
  void GoToAsking();
  void GoToShowingResult();
  void GoToGameOver();
  void GoToHistory();
  void GoToSessionDetail(int sessionIndex);

  void StartGame(Difficulty d);
  void SubmitAnswer(double selected);
  void GenerateNextQuestion();
  void DecrementTime(float dt);

  float                      GetTimeRemaining()      const { return _timeRemaining; }
  float                      GetTimerDuration()      const { return _timerDuration; }
  const Question*            GetCurrentQuestion()    const { return _current_question.get(); }
  bool                       WasLastAnswerCorrect()  const { return _was_correct; }
  int                        GetScore()              const { return _score; }
  int                        GetTotal()              const { return _total; }
  int                        GetSelectedSessionIndex() const { return _selectedSessionIndex; }
  const IHistoryRepository&  GetHistory()            const { return *_historyRepo; }

private:
  std::unique_ptr<Renderer>           _renderer;
  std::unique_ptr<Question>           _current_question;
  std::unique_ptr<IGameState>         _state;
  std::unique_ptr<IHistoryRepository> _historyRepo;
  Difficulty _difficulty          = Difficulty::Easy;
  bool       _was_correct         = false;
  float      _timeRemaining       = 0.0f;
  float      _timerDuration       = 0.0f;
  int        _score               = 0;
  int        _total               = 0;
  int        _selectedSessionIndex = 0;
};

}; // namespace noonoo

#endif // GAME_HPP
