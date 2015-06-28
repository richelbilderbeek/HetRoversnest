#include <cassert>
#include <iostream>
#include <iterator>
#include <sstream>
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
    const std::vector<std::string> lines = FileToVector(filename);
    std::stringstream s;
    s << std::noskipws;
    std::copy(std::begin(lines),std::end(lines),std::ostream_iterator<std::string>(s," "));

    //Show text until @
    {
      int pos = 0;
      char prev_c = ' ';
      while (s)
      {
        char c;
        s >> c;
        if (c == '@') break; //Now the options must be parsed
        if (c == '\n') c = ' '; //Convert a newline to space, so input files do not require a space after every line
        if (c == ' ' && pos == 0) continue; //Een nieuwe regel begint niet met een spatie
        if (c == ' ' && prev_c == ' ') continue; //Tweede spatie overslaan
        std::cout << c;
        prev_c = c;
        ++pos;
        if (pos > 60 && c == ' ')
        {
          std::cout << '\n';
          pos = 0;
        }
      }
    }
    std::cout << std::endl;
    char chapter_type;
    s >> chapter_type;
    switch (chapter_type)
    {
      case '0': DoNormalChapter(s,chapter); break;
      default: std::cout << "Chapter " << chapter << " does not have a code" << std::endl; return 1;
    }
  }
}
