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
  const Character character(6,12,6,Item::luck_potion);
  #ifndef NDEBUG
  if (1==2)
  {
    Walkthrough walkthrough(seed,character);
    walkthrough.Start();
    assert(!"Walkthrough worked");
  }
  #endif

  Dialog d;
  Game game(seed,character);
  d.ConnectTo(game);
  while (1)
  {
    game.DoChapter();
    if (game.HasWon() || game.HasLost()) break;
  }

}
