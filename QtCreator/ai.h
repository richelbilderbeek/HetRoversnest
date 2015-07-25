#ifndef AI_H
#define AI_H

#include <iosfwd>
#include <random>
#include <set>

#include "option.h"
#include "game.h"

///The articial intelligence
struct Ai
{
  using Key = std::string;
  using Payoff = double;
  using PayoffPair = std::pair<Key,Payoff>;

  Ai();
  Ai(const Ai& ai) = delete;
  Ai& operator=(const Ai& ai) = delete;

  ///Create a graph that shows the payoffs
  void CreateGraph() const noexcept;

  //Chapter of current sequence
  const auto& GetChapters() const noexcept { return m_keys; }

  //Payoff for each chapter
  const auto& GetPayoffs() const noexcept { return m_payoffs; }

  double GetPayoff(const std::string& option_text) const noexcept;

  void SetPayoff(const Key& chapter, const Payoff& payoff);

  void SetFinalPayoff(const Payoff& final_payoff);

  void Start();

  private:

  std::uniform_real_distribution<double> m_distribution;
  std::mt19937 m_engine;

  Game * m_game;

  ///All the keys (option texts) that were chosen
  std::vector<Key> m_keys;

  //Payoff for each key
  mutable std::vector<PayoffPair> m_payoffs;

  Option SlotRequestOption(const std::vector<Option>& options);
  void SlotShowText(const std::string& text);
  void SlotWait();

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

std::ostream& operator<<(std::ostream& os, const Ai& ai);

#endif // AI_H
