#include <ctime>
#include <fstream>
#include <iterator>
#include <memory>
#include "ai.h"
#include "dice.h"
#include "dialog.h"
#include "helper.h"
#include "game.h"
#include "walkthrough.h"

int main()
{
  #ifndef NDEBUG
  TestHelperFunctions();
  #endif

  if (1==2)
  {
    std::unique_ptr<Ai> ai(new Ai);
    ai->SolveGame();
  }


  //Play the game
  std::random_device rd;
  const int seed{static_cast<int>(rd())};
  const Character character(6+6,12+6,6+6,Item::luck_potion);

  Dialog d;
  Game game(seed,character);
  d.ConnectTo(game);
  while (1)
  {
    game.DoChapter();
    if (game.HasWon() || game.HasLost()) break;
  }

}
