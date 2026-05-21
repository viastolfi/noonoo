#ifndef IHISTORY_REPOSITORY_HPP
#define IHISTORY_REPOSITORY_HPP

#include "GameSession.hpp"
#include "QuestionRecord.hpp"
#include <vector>

namespace noonoo {

// Repository pattern: decouples history storage from its persistence strategy.
// Swap implementations (e.g. JSON, database) without touching Game or the states.
class IHistoryRepository {
public:
    virtual ~IHistoryRepository() = default;

    virtual void BeginSession(Difficulty d)             = 0;
    virtual void RecordQuestion(const QuestionRecord& q) = 0;
    virtual void EndSession(int score, int total)       = 0;

    virtual const std::vector<GameSession>& GetAllSessions() const = 0;
    virtual void Load()  = 0;
    virtual void Save() const = 0;
};

} // namespace noonoo

#endif // IHISTORY_REPOSITORY_HPP
