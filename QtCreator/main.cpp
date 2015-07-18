#include <cassert>
#include <ctime>
#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include "helper.h"
#include "dochapter.h"
#include "character.h"

int main()
{
  #ifndef NDEBUG
  Test();
  bool auto_play{true};
  std::srand(0);
  int chapter = 1;
  Character character(100,100,100,Item::luck_potion);

  //chapter = 8; std::srand(28); //Start
  //chapter = 206; //Dice game
  //chapter = 223; std::srand(0); //Pill game and die
  //chapter = 223; std::srand(2); //Pill game and live
  #else
  std::srand(std::time(0));
  int chapter = 1;
  Character character(12,12,12,Item::luck_potion);
  const bool auto_play{false};
  #endif

  const Language language = Language::English;

  while (1)
  {
    std::cout
      << '\n'
    #ifndef NDEBUG
      << std::string(60,'-') << '\n'
      << chapter << '\n'
      << std::string(60,'-') << '\n'
    #endif
    ;
    DoChapter(chapter,character,language,auto_play);
    if (character.IsDead() || chapter == 0)
    {
      DoGameOver();
      break;
    }
    if (chapter == 400) { break; }
  }

  std::ofstream f("Solution.txt");
  const auto v = character.GetChapters();
  std::copy(std::begin(v),std::end(v),std::ostream_iterator<int>(f," "));
}
