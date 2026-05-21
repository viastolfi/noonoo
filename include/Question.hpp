#ifndef QUESTION_HPP
#define QUESTION_HPP

#include "QuestionSign.hpp"

#include <vector>

namespace noonoo {

class Question {
public:
  Question(double lvalue, double rvalue, QuestionSign sign);
  ~Question() = default;

  double GetLValue() const;
  double GetRValue() const;
  QuestionSign GetSign() const;
  double GetResult() const;
private:
  double _lvalue;
  double _rvalue;
  double _result;
  QuestionSign _sign;
  std::vector<double> _possible_answer;
};

} // namespace noonoo

#endif // QUESTION_HPP
