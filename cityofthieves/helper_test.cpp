#include "helper.h"

#include <memory>
#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "ai.h"
#include "chapter.h"
#include "dice.h"
#include "game.h"
#include "item.h"

#ifndef NDEBUG
void TestHelperFunctions()
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

}
#endif
