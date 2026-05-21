#ifndef JSON_HISTORY_REPOSITORY_HPP
#define JSON_HISTORY_REPOSITORY_HPP

#include "IHistoryRepository.hpp"
#include <optional>
#include <string>

namespace noonoo {

class JsonHistoryRepository : public IHistoryRepository {
public:
    explicit JsonHistoryRepository(const std::string& filePath);

    void BeginSession(Difficulty d)              override;
    void RecordQuestion(const QuestionRecord& q) override;
    void EndSession(int score, int total)        override;

    const std::vector<GameSession>& GetAllSessions() const override;
    void Load()  override;
    void Save() const override;

private:
    std::string              _filePath;
    std::vector<GameSession> _sessions;
    std::optional<GameSession> _current;  // session in progress, not yet committed
};

} // namespace noonoo

#endif // JSON_HISTORY_REPOSITORY_HPP
