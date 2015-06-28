#include "helper.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include "character.h"
#include "dochapter.h"
void Test()
{
  //StripFirstChar
  {
    const std::vector<std::string> original = { "AB" };
    const std::vector<std::string> result = StripFirstChar(original);
    const std::vector<std::string> expected = { "B" };
    assert(result == expected);
  }
  //StripFirstChar
  {
    const std::vector<std::string> original = { "AB", "CDEF" };
    const std::vector<std::string> result = StripFirstChar(original);
    const std::vector<std::string> expected = { "B", "CDEF" };
    assert(result == expected);
  }
  //Try all chapters
  Character character(100,100,100,Item::shield);
  for (int i=1; i!=400; ++i)
  {
    try
    {
      int chapter = i; //Must use copy, otherwise i is changed
      DoChapter(chapter,character,true);
    }
    catch (std::runtime_error& e)
    {
      std::cout << e.what() << std::endl;
    }
  }
}

