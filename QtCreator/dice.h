#ifndef DICE_H
#define DICE_H

#include <random>

struct Dice
{
  static Dice * Get();

  auto& GetEngine() noexcept { return m_engine; }
  void SetSeed(const int seed);
  int Throw() noexcept;

  private:
  Dice();
  static Dice * m_instance;

  std::uniform_int_distribution<int> m_distribution;
  std::mt19937 m_engine;
};

#endif // DICE_H
