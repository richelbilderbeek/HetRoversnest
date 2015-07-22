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
#include "specialchapter.h"

#ifndef NDEBUG
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
  //Test if game runs identically twice with same RNG seed
  {
    const int seed{123456};
    Dice::Get()->SetSeed(seed);

    Character character1(
      Dice::Get()->Throw(),
      Dice::Get()->Throw() + Dice::Get()->Throw(),
      Dice::Get()->Throw(),
      Item::luck_potion
    );

    const Game game1(seed,character1);

    Dice::Get()->SetSeed(seed);

    Character character2(
      Dice::Get()->Throw(),
      Dice::Get()->Throw() + Dice::Get()->Throw(),
      Dice::Get()->Throw(),
      Item::luck_potion
    );

    const Game game2(seed,character2);
    assert(character1.GetChapters() == character2.GetChapters());
  }

}
#endif
