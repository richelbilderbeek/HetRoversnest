#include "game.h"

#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>

#include "character.h"
#include "helper.h"
#include "chapter.h"
#include "specialchapter.h"

Game::Game(
  const int rng_seed,
  const bool auto_play)
{
  std::srand(rng_seed);
  #ifndef NDEBUG
  Character character(100,100,100,Item::luck_potion);
  #else
  std::srand(std::time(0)); //Mix up RNG
  Character character(100,100,100,Item::luck_potion);
  #endif

  while (1)
  {
    const int chapter_number{character.GetCurrentChapter()};
    if (chapter_number == 400) { break; }
    std::cout
      << '\n'
    #ifndef NDEBUG
      << std::string(60,'-') << '\n'
      << chapter_number << '\n'
      << std::string(60,'-') << '\n'
    #endif
    ;
    const std::string filename{"../Files/" + std::to_string(chapter_number) + ".txt"};
    if (!IsRegularFile(filename))
    {
      std::stringstream msg;
      msg << __func__ << ": ERROR: File " << filename << " does not exist";
      throw std::runtime_error(msg.str());
    }
    const Chapter chapter(filename);
    chapter.Do(character,auto_play);
    if (character.IsDead())
    {
      DoGameOver();
      return;
    }
  }
  std::ofstream f("Solution.txt");
  const auto v = character.GetChapters();
  std::copy(std::begin(v),std::end(v),std::ostream_iterator<int>(f," "));
  f << "\nSeed: " << rng_seed << '\n';
}
