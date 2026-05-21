#include "Question.hpp"

#include <algorithm>
#include <random>
#include <set>
#include <string>

namespace noonoo {

Question::Question(double lvalue, double rvalue, QuestionSign sign)
  :_lvalue(lvalue), _rvalue(rvalue), _sign(sign)
{
  switch (sign) {
    case QuestionSign::SIGN_PLUS:
      _result = lvalue + rvalue;
      break;
    case QuestionSign::SIGN_MINUS:
      _result = lvalue - rvalue;
      break;
    case QuestionSign::SIGN_MUL:
      _result = lvalue * rvalue;
      break;
    case QuestionSign::SIGN_DIV:
      _result = lvalue / rvalue;
      break;
    default:
      // TODO: add exception handling
    break;
  }

  static std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<int> offsetDist(1, 9);
  std::uniform_int_distribution<int> signDist(0, 1);

  std::set<int> used;
  used.insert((int)_result);
  _possible_answer = { _result };

  for (int attempt = 0; _possible_answer.size() < 3 && attempt < 200; attempt++) {
    int sign = signDist(rng) == 0 ? 1 : -1;
    int candidate = (int)_result + sign * offsetDist(rng);
    if (candidate >= 0 && used.find(candidate) == used.end()) {
      used.insert(candidate);
      _possible_answer.push_back((double)candidate);
    }
  }

  std::shuffle(_possible_answer.begin(), _possible_answer.end(), rng);
}

double Question::GetLValue() const
{
  return _lvalue;
}

double Question::GetRValue() const
{
  return _rvalue;
}

QuestionSign Question::GetSign() const
{
  return _sign;
}

double Question::GetResult() const
{
  return _result;
}

const std::vector<double>& Question::GetPossibleAnswers() const
{
  return _possible_answer;
}

std::string Question::ToString() const
{
  const char* sign = "?";
  switch (_sign) {
    case QuestionSign::SIGN_PLUS:  sign = "+"; break;
    case QuestionSign::SIGN_MINUS: sign = "-"; break;
    case QuestionSign::SIGN_MUL:   sign = "x"; break;
    case QuestionSign::SIGN_DIV:   sign = "/"; break;
  }
  return std::to_string((int)_lvalue) + " " + sign + " " +
         std::to_string((int)_rvalue) + " = ?";
}

} // namespace noonoo
