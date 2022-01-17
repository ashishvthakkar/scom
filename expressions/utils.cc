#include "utils.h"

#include "alg_terms.h"
#include "expression.h"

namespace code_experiments {

int Compute(const std::string &expression) {
  return Expression<int>(expression).Eval();
}

std::string Simplify(const std::string &expression) {
  return Expression<AlgTerms>(expression).Eval().ToString();
}

} // namespace code_experiments