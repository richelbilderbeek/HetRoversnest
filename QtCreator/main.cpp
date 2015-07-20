#include <ctime>
#include <fstream>
#include <iterator>

#include "helper.h"
#include "game.h"

int main()
{
  #ifndef NDEBUG
  Test();
  #endif

  #ifndef NDEBUG
  for (int rng_seed = 0; ; ++rng_seed)
  {
    { std::ofstream f("last_seed.txt"); f << rng_seed; }

    std::srand(rng_seed);

    Character character(
      1 + (std::rand() % 12),
      1 + (std::rand() % 12),
      1 + (std::rand() % 12),
      Item::luck_potion
    );

    const Game game(rng_seed,character,true);

    if (game.HasWon())
    {
      std::ofstream f("Solution.txt");
      const auto v = game.GetCharacter().GetChapters();
      std::copy(std::begin(v),std::end(v),std::ostream_iterator<int>(f," "));
      f << "\nSeed: " << rng_seed << '\n';
      break;
    }
  }
  #endif

  //Play the game
  std::srand(std::time(0)); //Mix up RNG
  const int seed{std::rand()};
  Character character(12,12,12,Item::luck_potion);
  Game(seed,character,false);
}
