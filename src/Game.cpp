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
  _currentLevel = PlayerLevel::Compute(_historyRepo->GetTotalCorrect());
  _renderer->SetMascotLevel(_currentLevel);
}

Game::~Game() = default;

void Game::Run()
{
  while (!_renderer->ShouldWindowClose())
  {
    float dt = _renderer->GetDeltaTime();
    _mascot.Update(dt, (float)_renderer->GetWidth(), (float)_renderer->GetHeight());

    _renderer->BeginDraw();
    _renderer->DrawMascot(_mascot);
    _renderer->DrawBadge(GetTotalCorrect());
    _state->Update(*this, *_renderer);

    bool toggleClicked = false;
    _renderer->DrawMascotToggle(_mascot.IsVisible(), toggleClicked);
    if (toggleClicked) _mascot.ToggleVisible();

    _renderer->EndDraw();
  }
}

void Game::GoToSelectingDifficulty() { _state = std::make_unique<SelectingDifficultyState>(); }
void Game::GoToAsking()              { _state = std::make_unique<AskingState>(); }
void Game::GoToShowingResult()       { _state = std::make_unique<ShowingResultState>(); }
void Game::GoToGameOver()
{
  _sessionActive = false;
  _historyRepo->EndSession(_score, _total);
  int newLevel = PlayerLevel::Compute(_historyRepo->GetTotalCorrect());
  _justLeveledUp = (newLevel != _currentLevel);
  if (_justLeveledUp) {
    _currentLevel = newLevel;
    _renderer->SetMascotLevel(_currentLevel);
  }
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
  _sessionActive  = true;
  _justLeveledUp  = false;
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

int Game::GetTotalCorrect() const
{
  // include current game's score only when a session is active to avoid
  // double-counting after EndSession has already committed it to the repository
  return _historyRepo->GetTotalCorrect() + (_sessionActive ? _score : 0);
}

}; // namespace noonoo
