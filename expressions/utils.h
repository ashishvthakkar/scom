#ifndef EXPERIMENTS_EXPRESSIONS_UTILS_H
#define EXPERIMENTS_EXPRESSIONS_UTILS_H

#include <string>

namespace code_experiments {

int Compute(const std::string &expression);
std::string Simplify(const std::string &expression);

} // namespace code_experiments

#endif