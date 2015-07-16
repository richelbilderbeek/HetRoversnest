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
  #endif

  int chapter = 1; std::srand(std::time(0));


  Character character(100,100,100,Item::luck_potion);

  //int chapter = 8; std::srand(28); //Start
  //int chapter = 206; //Dice game

  //int chapter = 223; std::srand(0); //Pill game and die
  //int chapter = 223; std::srand(2); //Pill game and live
  const bool auto_play{false};
  const Language language = Language::English;

  while (1)
  {
    std::cout
      << '\n'
      << std::string(60,'-') << '\n'
      << chapter << '\n'
      << std::string(60,'-') << '\n'
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
