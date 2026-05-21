#ifndef QUESTION_FACTORY_HPP
#define QUESTION_FACTORY_HPP

#include "Difficulty.hpp"
#include "Question.hpp"
#include <memory>

namespace noonoo {

class QuestionFactory {
public:
  static std::unique_ptr<Question> Create(Difficulty difficulty);
};

} // namespace noonoo

#endif // QUESTION_FACTORY_HPP
