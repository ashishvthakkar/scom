#ifndef EXPERIMENTS_EXPRESSIONS_OPERATOR_NODE_H
#define EXPERIMENTS_EXPRESSIONS_OPERATOR_NODE_H

#include <string>

namespace code_experiments {

template <typename T>
class OperatorNode {
public:
  OperatorNode(const std::string &left, char op, const std::string &right)
      : left_(left),
        operator_(op),
        right_(right) {}
  [[nodiscard]] T Eval() const;

private:
  const std::string &left_;
  char operator_;
  const std::string &right_;
};

}  // namespace code_experiments

#endif