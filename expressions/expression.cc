#include "expression.h"

#include <glog/logging.h>

#include "alg_terms.h"
#include "operator_node.h"

namespace code_experiments {

template <typename T>
int Expression<T>::GetOperatorIndex(int &exp_operator_index) const {
  exp_operator_index = -1;
  int mul_div_index = -1;
  int num_open_params = 0;
  for (int i = expr_.size() - 1; i >= 0; i--) {
    if (num_open_params == 0 && (expr_[i] == '+' || expr_[i] == '-')) {
      return i;
    }
    if (num_open_params == 0 && (expr_[i] == '*' || expr_[i] == '/')) {
      mul_div_index = i;
    } else if (num_open_params == 0 && expr_[i] == '^') {
      exp_operator_index = i;
    } else if (expr_[i] == ')') {
      num_open_params++;
    } else if (expr_[i] == '(') {
      num_open_params--;
    }
  }
  return mul_div_index;
}

template <typename T>
bool Expression<T>::HasEnclosingParens() const {
  if (expr_[0] == '(') {
    CHECK(expr_[expr_.size() - 1] == ')')
        << "Unexpected parenthesis start without end: " << expr_;
    return true;
  }
  return false;
}

template <typename T>
T Expression<T>::GetSimplifiedVal() const {
  return expr_.empty() ? 0 : std::stoi(expr_);
}

template <typename T>
T Expression<T>::Eval() const {
  int exp_operator_index = -1;
  int op_index = GetOperatorIndex(exp_operator_index);
  if (op_index != -1) {
    return OperatorNode<T>(
               expr_.substr(0, op_index),
               expr_[op_index],
               expr_.substr(op_index + 1))
        .Eval();
  }
  if (exp_operator_index != -1) {
    int exponent = std::stoi(expr_.substr(exp_operator_index + 1));
    T base(Expression(expr_.substr(0, exp_operator_index)).Eval());
    T result(base);
    for (int i = 1; i < exponent; i++) {
      result = result * base;
    }
    return result;
  }
  if (HasEnclosingParens()) {
    return Expression(expr_.substr(1, expr_.size() - 2)).Eval();
  }
  return GetSimplifiedVal();
}

template <>
AlgTerms Expression<AlgTerms>::GetSimplifiedVal() const {
  return AlgTerms(AlgTerm(expr_));
}

template class Expression<int>;
template class Expression<AlgTerms>;

}  // namespace code_experiments