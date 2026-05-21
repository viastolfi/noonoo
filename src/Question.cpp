#include "Question.hpp"

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

  // TODO: make this random and part of a factory
  _possible_answer.resize(3);
  _possible_answer = {34.0, _result, 12.0};
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

} // namespace noonoo
