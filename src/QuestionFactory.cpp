#include "QuestionFactory.hpp"

#include <random>

namespace noonoo {

std::unique_ptr<Question> QuestionFactory::Create()
{
  static std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<int> valueDist(1, 10);
  // Division excluded for now — reintroduced with difficulty levels to ensure whole-number results.
  std::uniform_int_distribution<int> signDist(0, 2);

  int lvalue = valueDist(rng);
  int rvalue = valueDist(rng);
  QuestionSign sign = static_cast<QuestionSign>(signDist(rng));

  return std::make_unique<Question>((double)lvalue, (double)rvalue, sign);
}

} // namespace noonoo
