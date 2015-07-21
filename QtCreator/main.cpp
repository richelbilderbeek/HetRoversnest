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

  {
    std::unique_ptr<Ai> ai(new Ai);
    ai->SolveGame();
  }

  //Play the game
  std::random_device rd;
  const int seed{static_cast<int>(rd())};
  Character character(12,12,12,Item::luck_potion);
  Game(seed,character);
}
