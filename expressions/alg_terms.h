#ifndef EXPERIMENTS_EXPRESSIONS_ALG_TERMS
#define EXPERIMENTS_EXPRESSIONS_ALG_TERMS

#include <map>
#include <string>

namespace code_experiments {

// NOTE: String manipulation below can be an area of perf optimization.

class AlgTerm {
public:
  explicit AlgTerm(const std::string &term);

  AlgTerm operator*(const AlgTerm &right) const;
  AlgTerm operator/(const AlgTerm &right) const;
  void operator*=(const AlgTerm &right);

  [[nodiscard]] std::string ToStringWithLead(
      const std::string &lead_for_positive) const;
  [[nodiscard]] AlgTerm NegativeForm() const;
  [[nodiscard]] AlgTerm ReciprocalExpr() const;
  [[nodiscard]] std::string GetRawExpr() const;
  void AddCoeffs(const AlgTerm &right);
  [[nodiscard]] bool IsZero() const;

private:
  [[nodiscard]] int GetSignedCoeff() const;

  bool is_negative_;
  int coeff_;
  std::map<std::string, int> numerator_;
  std::map<std::string, int> denominator_;
};

class AlgTerms {
public:
  explicit AlgTerms() = default;
  explicit AlgTerms(const AlgTerm &alg_term) {
    std::string raw_expr(alg_term.GetRawExpr());
    // Note: For using [] on the map, the value has to be default constructible.
    raw_expr_to_alg_term_.insert(std::make_pair(raw_expr, alg_term));
  }

  AlgTerms operator+(const AlgTerms &right) const;
  AlgTerms operator-(const AlgTerms &right) const;
  AlgTerms operator*(const AlgTerms &right) const;
  AlgTerms operator/(const AlgTerms &right) const;
  void AddTerm(const std::string &raw_expr, const AlgTerm &alg_term);

  [[nodiscard]] std::string ToString() const;

private:
  [[nodiscard]] AlgTerm UnifiedForm() const;

  std::map<std::string, AlgTerm> raw_expr_to_alg_term_;
};

}  // namespace code_experiments

#endif