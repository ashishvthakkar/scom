#include "operator_node.h"

#include <glog/logging.h>

#include "alg_terms.h"
#include "expression.h"

namespace code_experiments {

template <typename T>
T OperatorNode<T>::Eval() const {
  T left = Expression<T>(left_).Eval();
  T right = Expression<T>(right_).Eval();
  switch (operator_) {
    case '+':
      return left + right;
    case '-':
      return left - right;
    case '*':
      return left * right;
    case '/':
      return left / right;
    default:
      LOG_ASSERT(false) << "Unexpected operator: " << operator_;
  }
}

template class OperatorNode<int>;
template class OperatorNode<AlgTerms>;

} // namespace code_experiments