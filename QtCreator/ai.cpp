#include "ai.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>

#include "dice.h"
#include "game.h"
#include "helper.h"

Ai::Ai()
  : m_chapters{},
    m_distribution{std::uniform_real_distribution<double>(0.0,1.0)},
    m_engine(0),
    m_payoffs{std::vector<double>(450,-1.0)}
{
  #ifndef NDEBUG
  Test();
  #endif
}

double Ai::GetPayoff(const int chapter) const noexcept
{
  assert(chapter >= 0);
  assert(chapter < static_cast<int>(m_payoffs.size()));
  return m_payoffs[chapter];
}

int Ai::Select(const std::vector<Option>& options)
{
  std::vector<double> expected_payoffs;
  std::transform(
    std::begin(options),
    std::end(options),
    std::back_inserter(expected_payoffs),
    [this](const Option& option)
    {
      const int next_chapter{option.GetNextChapter()};
      if (GetPayoff(next_chapter) < 0.0) { SetPayoff(next_chapter,1.0); }
      const double payoff{GetPayoff(next_chapter)};
      assert(payoff >= 0.0);
      return payoff;
    }
  );

  const int n_options{static_cast<int>(options.size())};

  const bool choose_probabilisticly{true};
  if (choose_probabilisticly)
  {
    const double sum{std::accumulate(std::begin(expected_payoffs),std::end(expected_payoffs),0.0)};
    if (sum != 0.0)
    {
      double f{m_distribution(m_engine) * sum};
      for (int i=0; i!=n_options; ++i)
      {
        const int next_chapter{options[i].GetNextChapter()};
        if (f < expected_payoffs[next_chapter])
        {
          m_chapters.push_back(next_chapter);
          return i;
        }
        f -= expected_payoffs[ next_chapter ];
      }
      //Should not get here often
    }
    //Choose first option
    m_chapters.push_back(options[0].GetNextChapter());
    return 0;
  }
  //Choose best
  int best_option{0};
  double expected_payoff{GetPayoff(options[0].GetNextChapter())};
  for (int i=1; i!=n_options; ++i)
  {
    double this_expected_payoff{GetPayoff(options[i].GetNextChapter())};
    if (this_expected_payoff > expected_payoff)
    {
      best_option = i;
      expected_payoff = this_expected_payoff;
    }
  }
  m_chapters.push_back(options[best_option].GetNextChapter());
  return best_option;
}

void Ai::SetFinalPayoff(const double final_payoff)
{
  double weight{1.0};
  for (int i=0; ; ++i)
  {
    weight *= 0.1;

    if (m_chapters.empty()) break;
    const int chapter{m_chapters.back()};
    const double current_chapter_payoff{GetPayoff(chapter)};
    const double new_chapter_payoff{
      current_chapter_payoff + (weight * (final_payoff - current_chapter_payoff))
    };
    if (!IsBetween(new_chapter_payoff,current_chapter_payoff,final_payoff))
    {
      std::cerr
        << "Now: " << current_chapter_payoff << '\n'
        << "Final: " << final_payoff << '\n'
        << "Weight: " << weight<< '\n'
        << "New: " << new_chapter_payoff << '\n'
      ;
    }
    assert(IsBetween(new_chapter_payoff,current_chapter_payoff,final_payoff));
    SetPayoff(chapter,new_chapter_payoff);
    m_chapters.pop_back();
  }
  m_chapters.clear();
}

void Ai::SetPayoff(const int chapter, const double payoff)
{
  assert(payoff >= 0.0);
  assert(chapter >= 0);
  assert(chapter < static_cast<int>(m_payoffs.size()));
  m_payoffs[chapter] = payoff;
}

void Ai::SolveGame()
{
  std::cout << "SOLVING THE GAME" << std::endl;;

  Ai * const ai = new Ai;
  const int rng_seed{44};
  Dice::Get()->SetSeed(rng_seed);

  for (int i=1; ; ++i)
  {
    //if i equals 1,10,100,1000,etc...
    if (i % 10000 == 0)
    //if (static_cast<int>(std::log10(i)) != static_cast<int>(std::log10(i-1)))
    {
      std::cout << "i: " << i << std::endl;
      CreateGraph(ai);
      std::cout << "DONE\n";
    }
    Character character(
      6,
      12,
      6,
      Item::luck_potion
    );

    Game game(rng_seed,character);
    while (1)
    {
      game.DoChapter();
      if (game.HasLost() || game.HasWon()) break;
    }

    const double final_payoff{
        character.HasItem(Item::black_pearls) ? 2.0 : 0.0
      + character.HasItem(Item::lotus_flower) ? 2.0 : 0.0
      + character.HasItem(Item::hags_hair) ? 2.0 : 0.0
      + character.HasItem(Item::tattoo) ? 2.0 : 0.0
      + character.HasItem(Item::silver_arrow) ? 2.0 : 0.0
      + game.HasWon() ? 2.0 : 0.0
    };
    ai->SetFinalPayoff(final_payoff);
    if (game.HasWon()) { break; }
  }
  std::cout << "FINISHED THE GAME, CREATING GRAPH" << std::endl;
  CreateGraph(ai);
  delete ai;
}

#ifndef NDEBUG
void Ai::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
}
#endif

std::ostream& operator<<(std::ostream& os, const Ai& ai)
{
  const auto payoffs = ai.GetPayoffs();
  const int n_payoffs{static_cast<int>(payoffs.size())};
  for (int i=0; i!=n_payoffs; ++i)
  {
    os << i << " " << payoffs[i] << '\n';
  }
  return os;
}
