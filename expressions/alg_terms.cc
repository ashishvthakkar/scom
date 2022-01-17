#include "alg_terms.h"

#include <glog/logging.h>

#include <algorithm>

namespace code_experiments {

// Note: Performance for the below may be improved by reducing temporary copies
// that may be created despite RVO (for instance by using += instead of +).

static void AppendKeys(
    const std::map<std::string, int> &from_map,
    const std::string &delim_after_first,
    std::string &to_string) {
  std::string delim;
  for (const auto &element : from_map) {
    to_string.append(delim);
    to_string.append(element.first);
    if (element.second != 1) {
      to_string.append("^" + std::to_string(element.second));
    }
    delim = delim_after_first;
  };
}

static void AddMaps(
    const std::map<std::string, int> &to_add,
    std::map<std::string, int> &into) {
  for (const auto &element : to_add) {
    if (into.find(element.first) != into.end()) {
      into[element.first] += element.second;
    } else {
      into[element.first] = element.second;
    }
  }
}

AlgTerm::AlgTerm(const std::string &init_term)
    : coeff_(1),
      is_negative_(false) {
  int index = 0;
  if (!init_term.empty() && init_term[index] == '-') {
    is_negative_ = true;
    index++;
  }
  const std::string &term = index > 0 ? init_term.substr(index) : init_term;
  if (!term.empty() && term[0] >= '0' && term[0] <= '9') {
    std::string::size_type str_index = 0;
    coeff_ = std::stoi(term, &str_index);
    numerator_[term.substr(str_index)] = 1;
  } else {
    numerator_[term] = 1;
  }
}

std::string AlgTerm::GetRawExpr() const {
  std::string raw_expr;
  AppendKeys(numerator_, "*", raw_expr);
  if (!denominator_.empty()) {
    raw_expr.append("/");
    if (denominator_.size() > 1) {
      raw_expr.append("(");
    }
    AppendKeys(denominator_, "*", raw_expr);
    if (denominator_.size() > 1) {
      raw_expr.append(")");
    }
  }
  return raw_expr;
}

std::string AlgTerm::ToStringWithLead(
    const std::string &lead_for_positive) const {
  LOG_ASSERT(coeff_ != 0) << "Unexpected internal state with coeff 0.";
  std::string return_value(is_negative_ ? "-" : lead_for_positive);
  if (coeff_ != 1) {
    return_value.append(std::to_string(coeff_));
  }
  return return_value + GetRawExpr();
}

AlgTerm AlgTerm::NegativeForm() const {
  AlgTerm return_value(*this);
  return_value.is_negative_ = !return_value.is_negative_;
  return return_value;
}

void AlgTerm::operator*=(const AlgTerm &right) {
  coeff_ *= right.coeff_;
  AddMaps(right.numerator_, numerator_);
  AddMaps(right.denominator_, denominator_);
  is_negative_ = is_negative_ != right.is_negative_;
  // NOTE: This currently does not simplify across numerator and denominator.
  // It also does not simplify x^0 to 1.
}

AlgTerm AlgTerm::operator*(const AlgTerm &right) const {
  AlgTerm product(*this);
  product *= right;
  return product;
}

AlgTerm AlgTerm::ReciprocalExpr() const {
  AlgTerm reciprocal(*this);
  reciprocal.numerator_.swap(reciprocal.denominator_);
  return reciprocal;
}

AlgTerm AlgTerm::operator/(const AlgTerm &right) const {
  AlgTerm quotient(*this);
  quotient *= right.ReciprocalExpr();
  quotient.coeff_ = coeff_ / right.coeff_;
  return quotient;
}

int AlgTerm::GetSignedCoeff() const {
  int sign = is_negative_ ? -1 : 1;
  return sign * coeff_;
}

void AlgTerm::AddCoeffs(const AlgTerm &right) {
  coeff_ = GetSignedCoeff() + right.GetSignedCoeff();
  if (coeff_ < 0) {
    is_negative_ = true;
    coeff_ = abs(coeff_);
  } else {
    is_negative_ = false;
  }
}

bool AlgTerm::IsZero() const { return coeff_ == 0; }

void AlgTerms::AddTerm(const std::string &raw_expr, const AlgTerm &alg_term) {
  if (raw_expr_to_alg_term_.find(raw_expr) != raw_expr_to_alg_term_.end()) {
    AlgTerm &result_alg_term = raw_expr_to_alg_term_.at(raw_expr);
    result_alg_term.AddCoeffs(alg_term);
    if (result_alg_term.IsZero()) {
      raw_expr_to_alg_term_.erase(raw_expr);
    }
  } else {
    raw_expr_to_alg_term_.insert(std::make_pair(raw_expr, alg_term));
  }
}

AlgTerms AlgTerms::operator+(const AlgTerms &right) const {
  AlgTerms sum(*this);
  for (const auto &element : right.raw_expr_to_alg_term_) {
    sum.AddTerm(element.first, element.second);
  }
  return sum;
}

AlgTerms AlgTerms::operator-(const AlgTerms &right) const {
  AlgTerms return_value(*this);
  for (const auto &element : right.raw_expr_to_alg_term_) {
    return_value.AddTerm(element.first, element.second.NegativeForm());
  }
  return return_value;
}

AlgTerms AlgTerms::operator*(const AlgTerms &right) const {
  AlgTerms product;
  for (const auto &left_element : raw_expr_to_alg_term_) {
    for (const auto &right_element : right.raw_expr_to_alg_term_) {
      AlgTerm term_product = left_element.second * right_element.second;
      product.AddTerm(term_product.GetRawExpr(), term_product);
    }
  }
  return product;
}

AlgTerm AlgTerms::UnifiedForm() const {
  CHECK(!raw_expr_to_alg_term_.empty())
      << "Unexpected scenario with no algebraic terms";
  if (raw_expr_to_alg_term_.size() == 1) {
    return raw_expr_to_alg_term_.begin()->second;  // Value of first element
  }
  std::string result("(");
  result.append(ToString());
  result.append(")");
  return AlgTerm(result);
}

AlgTerms AlgTerms::operator/(const AlgTerms &right) const {
  AlgTerms result;
  AlgTerm unified_right_term(right.UnifiedForm());
  for (const auto &left_element : raw_expr_to_alg_term_) {
    // NOTE: The below can be optimized to remove repeated reciprocal
    // computation.
    result.raw_expr_to_alg_term_.insert(std::make_pair(
        left_element.first,
        left_element.second / unified_right_term));
  }
  return result;
}

std::string AlgTerms::ToString() const {
  if (raw_expr_to_alg_term_.empty()) {
    return "";
  }
  std::string return_value;
  std::string lead_for_positive;
  for (const auto &element : raw_expr_to_alg_term_) {
    return_value.append(element.second.ToStringWithLead(lead_for_positive));
    lead_for_positive = "+";
  }
  return return_value;
}

} // namespace code_experiments