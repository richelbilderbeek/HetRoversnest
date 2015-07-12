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
  character.AddItem(Item::silver_arrow);
  int chapter = 36;
  const bool auto_play{false};
  const Language language = Language::English;

  while (1)
  {
    std::cout << std::string(60,'-') << '\n';
    std::cout << chapter << '\n';
    std::cout << std::string(60,'-') << '\n';
    DoChapter(chapter,character,language,auto_play);
    if (chapter == 0) break;
  }
}
