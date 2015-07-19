#include <fstream>

#include "chapter.h"
#include "dochapter.h"
#include "character.h"

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
    Game(rng_seed,true);
  }
  #else
  Game(86,false);
  #endif


}
