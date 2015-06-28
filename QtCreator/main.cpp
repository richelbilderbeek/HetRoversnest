#include <cassert>
#include <iostream>
#include <fstream>
#include "helper.h"
#include "dochapter.h"

int main()
{
  #ifndef NDEBUG
  Test();
  #endif

  int chapter = 1;

  while (1)
  {
    std::cout << std::string(60,'-') << '\n';
    std::cout << chapter << '\n';
    std::cout << std::string(60,'-') << '\n';

    const std::string filename{"../Bestanden/" + std::to_string(chapter) + ".txt"};
    assert(IsRegularFile(filename));
    const std::vector<std::string> lines_full = FileToVector(filename);
    assert(!lines_full.empty());
    const std::vector<std::string> lines = StripFirstChar(lines_full);
    const char code = lines_full[0][0];
    switch (code)
    {
      case '0': DoNormalChapter(lines,chapter); break;
      default: std::cout << "Chapter " << chapter << " does not have a code" << std::endl; return 1;
    }
  }
}
