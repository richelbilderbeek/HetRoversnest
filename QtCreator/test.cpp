#include "helper.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include "chapter.h"
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
  {
    Chapter chapter("../Files/1323.txt");
    assert(chapter.GetMonsters().size() == 1);
    //std::cout << chapter << std::endl;
  }
  //New fighting engine
  {
    Character character(100,100,100,Item::shield);
    character.AddItem(Item::silver_arrow);
    int chapter = 5;
    DoChapter(chapter,character,Language::English,true);
  }
  //Chapter 323: blacksmith must attack with attack strength 3
  if (1==2)
  {
    Character character(1,3,1,Item::luck_potion);
    character.SetChapter(323);
    int chapter = 323;
    DoChapter(chapter,character,Language::English,true);
    assert(character.GetStamina() == 0);
  }
  //Chapter 253: snakes must bite with attack strength 4
  //Chapter 429: lizardine has extra attack


  //Try all chapters
  Character character(100,100,100,Item::shield);
  character.AddItem(Item::silver_arrow);
  for (int i=1; i!=450; ++i)
  {
    for (const Language language: { Language::Dutch, Language::English } )
    {
      int chapter = i; //Must use copy, otherwise i is changed
      try
      {
        DoChapter(chapter,character,language,true);
        if (character.GetGold() < 30) { character.ChangeGold(30); }
      }
      catch (std::runtime_error& e)
      {
        std::cout << e.what() << std::endl;
      }
    }
  }
  //Try chapters of different types
}

