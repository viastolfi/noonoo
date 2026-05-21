#ifndef QUESTION_FACTORY_HPP
#define QUESTION_FACTORY_HPP

#include "Question.hpp"
#include <memory>

namespace noonoo {

class QuestionFactory {
public:
  static std::unique_ptr<Question> Create();
};

} // namespace noonoo

#endif // QUESTION_FACTORY_HPP
