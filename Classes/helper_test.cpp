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
void Helper::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  const Helper h;
  const bool verbose{false};
  if (verbose) { std::clog << __func__ << std::endl; }
  if (verbose) { std::clog << "StripFirstChar" << std::endl; }
  {
    const std::vector<std::string> original = { "AB" };
    const std::vector<std::string> result = h.StripFirstChar(original);
    const std::vector<std::string> expected = { "B" };
    assert(result == expected);
  }
  if (verbose) { std::clog << "StripFirstChar" << std::endl; }
  {
    const std::vector<std::string> original = { "AB", "CDEF" };
    const std::vector<std::string> result = h.StripFirstChar(original);
    const std::vector<std::string> expected = { "B", "CDEF" };
    assert(result == expected);
  }
  if (verbose) { std::clog << "IsInt" << std::endl; }
  {
    assert(!h.IsInt("2+fire"));
  }
  const bool test_timer{false};
  if (test_timer)
  {
    if (verbose) { std::clog << "Wait" << std::endl; }
    {
      for (int i=0; i!=10; ++i)
      {
        if (verbose) { std::clog << "Waiting for 100 msecs" << std::endl; }
        h.Wait(0.1);
      }
      for (int i=0; i!=3; ++i)
      {
        if (verbose) { std::clog << "Waiting for 1000 msecs" << std::endl; }
        h.Wait(1.0);
      }
    }
  }
  if (verbose) { std::clog << "Finished " << __func__ << std::endl; }
}
#endif
