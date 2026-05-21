#ifndef QUESTION_RECORD_HPP
#define QUESTION_RECORD_HPP

#include "Difficulty.hpp"
#include <string>

namespace noonoo {

struct QuestionRecord {
    std::string questionText;
    int         correctAnswer;
    int         userAnswer;
    bool        wasCorrect;
    Difficulty  difficulty;
};

} // namespace noonoo

#endif // QUESTION_RECORD_HPP
