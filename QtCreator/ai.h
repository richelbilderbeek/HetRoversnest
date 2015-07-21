#ifndef AI_H
#define AI_H

#include <iosfwd>
#include <random>
#include <set>

#include "option.h"

///The articial intellicence
struct Ai
{
  Ai();

  //Chapter of current sequence
  const auto& GetChapters() const noexcept { return m_chapters; }

  //Payoff for each chapter
  const auto& GetPayoffs() const noexcept { return m_payoffs; }

  double GetPayoff(const int chapter) const noexcept;

  void SetPayoff(const int chapter, const double payoff);

  void SetFinalPayoff(const double final_payoff);

  int Select(const std::vector<Option>& options);

  private:
  //Chapter of current sequence
  std::vector<int> m_chapters;

  std::uniform_real_distribution<double> m_distribution;
  std::mt19937 m_engine;

  //Payoff for each chapter
  std::vector<double> m_payoffs;



  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

std::ostream& operator<<(std::ostream& os, const Ai& ai);

#endif // AI_H
