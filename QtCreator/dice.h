#ifndef DICE_H
#define DICE_H

#include <random>

///A Singleton
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

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

#endif // DICE_H
