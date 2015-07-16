#include "helper.h"

#include <cassert>
#include <fstream>
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
  //New fighting engine
  //Sequential fight
  {
    const Chapter chapter("../Files/5.txt");
    assert(chapter.DoFightSequentially());
    assert(chapter.GetMonsters().size() == 1);
    assert(chapter.GetMonsters()[0].GetAttackDamage() == 2);
    Character character(100,100,100,Item::shield);
    chapter.Do(character,true);
  }
  {
    const Chapter chapter("../Files/10.txt");
    assert(chapter.DoFightSequentially());
    assert(chapter.GetMonsters().size() == 1);
    Character character(100,100,100,Item::shield);
    chapter.Do(character,true);
  }
  //Chapter 326: Simulateous fight
  {
    const Chapter chapter("../Files/326.txt");
    assert(!chapter.DoFightSequentially());
    assert(chapter.GetMonsters().size() == 2);
    Character character(100,100,100,Item::shield);
    chapter.Do(character,true);
  }
  //Chapter 323: blacksmith must attack with attack strength 3
  {
    const Chapter chapter("../Files/323.txt");
    assert(chapter.GetMonsters()[0].GetAttackDamage() == 3);
    //std::cout << chapter << std::endl;
  }
  //Chapter 253: snakes must bite with attack strength 4
  {
    const Chapter chapter("../Files/253.txt");
    assert(chapter.GetMonsters()[0].GetAttackDamage() == 4);
  }
  //Chapter 140: can escape after 3 rounds
  {
    const Chapter chapter("../Files/140.txt");
    assert(chapter.GetRoundsToEscape() == 3);
    assert(chapter.GetEscapeToChapter() == 282);
  }
  //Chapter 9: Game over
  {
    const Chapter chapter("../Files/9.txt");
    assert(chapter.IsGameOver());
  }
  //Chapter 400: Game won
  {
    const Chapter chapter("../Files/400.txt");
    assert(chapter.IsGameWon());
  }
  //Chapter 8: change status
  {
    const Chapter chapter("../Files/8.txt");
    assert(chapter.GetAddItems().count(Item::golden_brooch));
    assert(chapter.GetChangeLuck() == 2);
  }
  //Chapters 13 and 273: should not be able to take both brooches

  //Chapter 43,175,209: cannot lift a globlet multiple times

  //Chapter 429: lizardine has extra attack

  //All chapters with monster must have a valid next_chapter

  //Parse chapters using Chapter
  std::ofstream f("TODO.txt");
  for (int i=1; i!=450; ++i)
  {
    try
    {
      const Chapter chapter("../Files/" + std::to_string(i) + ".txt");
      if (chapter.GetNextChapter() == 1)
      {
        f << i << "ERROR: incorrect Next_chapter" << std::endl;
      }
      else
      {
        f << i << ": OK" << std::endl;
      }
    }
    catch (std::logic_error& e)
    {
      f << i << ": FAIL" << std::endl;
    }
    catch (std::runtime_error& e)
    {
      f << i << ": not present" << std::endl;
    }
  }
  //Try all chapters
  for (const Language language: { Language::Dutch, Language::English } )
  {
    for (int i=1; i!=450; ++i)
    {
      std::cout << "CHAPTER " << i << std::endl;
      Character character(100,100,100,Item::shield);
      character.AddItem(Item::silver_arrow);
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

