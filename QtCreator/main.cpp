#include <cassert>
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

  Character character(100,100,100,Item::shield);
  int chapter = 69;
  const bool auto_play{false};

  while (1)
  {
    std::cout << std::string(60,'-') << '\n';
    std::cout << chapter << '\n';
    std::cout << std::string(60,'-') << '\n';
    DoChapter(chapter,character,auto_play);
  }
}
