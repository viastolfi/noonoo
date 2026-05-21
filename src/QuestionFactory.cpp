#include "QuestionFactory.hpp"

#include <algorithm>
#include <random>
#include <vector>

namespace noonoo {

std::unique_ptr<Question> QuestionFactory::Create(Difficulty difficulty)
{
  static std::mt19937 rng(std::random_device{}());

  int maxValue;
  std::vector<QuestionSign> availableSigns;

  switch (difficulty) {
    case Difficulty::Easy:
      maxValue = 10;
      availableSigns = { QuestionSign::SIGN_PLUS, QuestionSign::SIGN_MINUS };
      break;
    case Difficulty::Medium:
      maxValue = 20;
      availableSigns = { QuestionSign::SIGN_PLUS, QuestionSign::SIGN_MINUS, QuestionSign::SIGN_MUL };
      break;
    case Difficulty::Hard:
      maxValue = 50;
      availableSigns = { QuestionSign::SIGN_PLUS, QuestionSign::SIGN_MINUS, QuestionSign::SIGN_MUL, QuestionSign::SIGN_DIV };
      break;
  }

  std::uniform_int_distribution<int> valueDist(1, maxValue);
  std::uniform_int_distribution<int> signDist(0, (int)availableSigns.size() - 1);

  QuestionSign sign = availableSigns[signDist(rng)];
  int lvalue, rvalue;

  if (sign == QuestionSign::SIGN_MUL) {
    // cap multiplication operands to avoid unreasonably large products.
    int mulMax = std::min(maxValue, 12);
    std::uniform_int_distribution<int> mulDist(1, mulMax);
    lvalue = mulDist(rng);
    rvalue = mulDist(rng);
  } else if (sign == QuestionSign::SIGN_DIV) {
    // pick rvalue then a multiplier so the result is always a whole number.
    std::uniform_int_distribution<int> rvalDist(2, 9);
    std::uniform_int_distribution<int> multDist(1, 10);
    rvalue = rvalDist(rng);
    lvalue = rvalue * multDist(rng);
  } else {
    lvalue = valueDist(rng);
    rvalue = valueDist(rng);
    // ensure subtraction never yields a negative result.
    if (sign == QuestionSign::SIGN_MINUS && lvalue < rvalue)
      std::swap(lvalue, rvalue);
  }

  return std::make_unique<Question>((double)lvalue, (double)rvalue, sign);
}

} // namespace noonoo
