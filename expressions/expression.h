#ifndef EXPERIMENTS_EXPRESSIONS_EXPRESSION_H
#define EXPERIMENTS_EXPRESSIONS_EXPRESSION_H

#include <string>

namespace code_experiments {

template <typename T>
class Expression {
public:
  explicit Expression(const std::string &expr) : expr_(expr) {}
  [[nodiscard]] T Eval() const;

private:
  [[nodiscard]] int GetOperatorIndex(int &exp_operator_index) const;
  [[nodiscard]] bool HasEnclosingParens() const;
  [[nodiscard]] T GetSimplifiedVal() const;

  const std::string &expr_;
};

}  // namespace code_experiments

#endif