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

  if (1==2)
  {
    std::unique_ptr<Ai> ai(new Ai);
    ai->SolveGame();
  }

  //Play the game
  std::random_device rd;
  const int seed{static_cast<int>(rd())};
  const Character character(12,12,12,Item::luck_potion);
  Game game(seed,character);
  while (1)
  {
    game.DoChapter();
    if (game.HasWon() || game.HasLost()) break;
  }

}
