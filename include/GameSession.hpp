#ifndef GAME_SESSION_HPP
#define GAME_SESSION_HPP

#include "Difficulty.hpp"
#include "QuestionRecord.hpp"
#include <vector>

namespace noonoo {

struct GameSession {
    Difficulty                  difficulty;
    int                         score;
    int                         total;
    std::vector<QuestionRecord> questions;
};

} // namespace noonoo

#endif // GAME_SESSION_HPP
