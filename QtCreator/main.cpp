#include <cassert>
#include <ctime>
#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <stdexcept>

#include "chapter.h"
#include "helper.h"
#include "dochapter.h"
#include "character.h"

int main()
{
  #ifndef NDEBUG
  Test();
  bool auto_play{true};
  std::srand(7);
  Character character(100,100,100,Item::luck_potion);
  #else
  std::srand(std::time(0));
  int chapter = 1;
  Character character(12,12,12,Item::luck_potion);
  const bool auto_play{false};
  #endif

  const Language language = Language::English;

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

    const std::string filename{
      (language == Language::Dutch ? "../Bestanden/" : "../Files/")
      + std::to_string(chapter_number) + ".txt"
    };
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
      break;
    }
  }

  std::ofstream f("Solution.txt");
  const auto v = character.GetChapters();
  std::copy(std::begin(v),std::end(v),std::ostream_iterator<int>(f," "));
}
