#include <ctime>
#include <fstream>
#include <iterator>
#include <memory>
#include "ai.h"
#include "dice.h"
#include "helper.h"
#include "game.h"

int main()
{
  #ifndef NDEBUG
  Test();
  #endif

  #ifndef NDEBUG
  std::unique_ptr<Ai> ai(new Ai);
  std::random_device rd_test;
  for (int rng_seed = static_cast<int>(rd_test()); ; ++rng_seed)
  {
    Dice::Get()->SetSeed(rng_seed);

    Character character(
      Dice::Get()->Throw(),
      Dice::Get()->Throw() + Dice::Get()->Throw(),
      Dice::Get()->Throw(),
      Item::luck_potion
    );

    const Game game(rng_seed,character,ai.get());

    if (game.HasWon())
    {
      std::ofstream f("Solution.txt");
      const auto v = game.GetCharacter().GetChapters();
      std::copy(std::begin(v),std::end(v),std::ostream_iterator<int>(f," "));
      f << "\nSeed: " << rng_seed << '\n';
      return 0;
    }
  }

  //Play the game
  std::random_device rd;
  const int seed{static_cast<int>(rd())};
  Character character(12,12,12,Item::luck_potion);
  Game(seed,character,ai.get());

  #else
  //Release
  //Play the game
  std::random_device rd;
  const int seed{static_cast<int>(rd())};
  Character character(12,12,12,Item::luck_potion);
  Game(seed,character);
  #endif

}
