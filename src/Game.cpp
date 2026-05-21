#include "Game.hpp"
#include "JsonHistoryRepository.hpp"
#include "QuestionFactory.hpp"
#include "states/AskingState.hpp"
#include "states/GameOverState.hpp"
#include "states/HistoryState.hpp"
#include "states/SelectingDifficultyState.hpp"
#include "states/SessionDetailState.hpp"
#include "states/ShowingResultState.hpp"

namespace noonoo {

static float timerDuration(Difficulty d)
{
  switch (d) {
    case Difficulty::Easy:   return 90.0f;
    case Difficulty::Medium: return 60.0f;
    case Difficulty::Hard:   return 45.0f;
  }
  return 60.0f;
}

Game::Game(std::unique_ptr<Renderer> r)
  : _renderer(std::move(r))
  , _state(std::make_unique<SelectingDifficultyState>())
  , _historyRepo(std::make_unique<JsonHistoryRepository>("history.json"))
{
  _historyRepo->Load();
}

Game::~Game() = default;

void Game::Run()
{
  while (!_renderer->ShouldWindowClose())
  {
    _renderer->BeginDraw();
    _state->Update(*this, *_renderer);
    _renderer->EndDraw();
  }
}

void Game::GoToSelectingDifficulty() { _state = std::make_unique<SelectingDifficultyState>(); }
void Game::GoToAsking()              { _state = std::make_unique<AskingState>(); }
void Game::GoToShowingResult()       { _state = std::make_unique<ShowingResultState>(); }
void Game::GoToGameOver()
{
  _historyRepo->EndSession(_score, _total);
  _state = std::make_unique<GameOverState>();
}
void Game::GoToHistory()             { _state = std::make_unique<HistoryState>(); }
void Game::GoToSessionDetail(int sessionIndex)
{
  _selectedSessionIndex = sessionIndex;
  _state = std::make_unique<SessionDetailState>();
}

void Game::StartGame(Difficulty d)
{
  _difficulty     = d;
  _timeRemaining  = timerDuration(d);
  _timerDuration  = _timeRemaining;
  _score          = 0;
  _total          = 0;
  _historyRepo->BeginSession(d);
  _current_question = QuestionFactory::Create(_difficulty);
  GoToAsking();
}

void Game::SubmitAnswer(double selected)
{
  _was_correct = ((int)selected == (int)_current_question->GetResult());
  if (_was_correct) _score++;
  _total++;

  QuestionRecord record{
    _current_question->ToString(),
    (int)_current_question->GetResult(),
    (int)selected,
    _was_correct,
    _difficulty
  };
  _historyRepo->RecordQuestion(record);
}

void Game::GenerateNextQuestion()
{
  _current_question = QuestionFactory::Create(_difficulty);
}

void Game::DecrementTime(float dt)
{
  _timeRemaining -= dt;
}

}; // namespace noonoo
