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
    assert(chapter.GetFighting().DoFightSequentially());
    assert(chapter.GetFighting().GetMonsters().size() == 1);
    assert(chapter.GetFighting().GetMonsters()[0].GetAttackDamage() == 2);
    Character character(100,100,100,Item::shield);
    chapter.Do(character,true);
  }
  {
    const Chapter chapter("../Files/10.txt");
    assert(chapter.GetFighting().DoFightSequentially());
    assert(chapter.GetFighting().GetMonsters().size() == 1);
    Character character(100,100,100,Item::shield);
    chapter.Do(character,true);
  }
  //Chapter 326: Simulateous fight
  {
    const Chapter chapter("../Files/326.txt");
    assert(!chapter.GetFighting().DoFightSequentially());
    assert(chapter.GetFighting().GetMonsters().size() == 2);
    Character character(100,100,100,Item::shield);
    chapter.Do(character,true);
  }
  //Chapter 323: blacksmith must attack with attack strength 3
  {
    const Chapter chapter("../Files/323.txt");
    assert(chapter.GetFighting().GetMonsters()[0].GetAttackDamage() == 3);
    //std::cout << chapter << std::endl;
  }
  //Chapter 253: snakes must bite with attack strength 4
  {
    const Chapter chapter("../Files/253.txt");
    assert(chapter.GetFighting().GetMonsters()[0].GetAttackDamage() == 4);
  }
  //Chapter 140: can escape after 3 rounds
  {
    const Chapter chapter("../Files/140.txt");
    assert(chapter.GetFighting().GetRoundsToEscape() == 3);
    assert(chapter.GetFighting().GetEscapeToChapter() == 282);
  }
  //Chapter 9: Game over
  {
    const Chapter chapter("../Files/9.txt");
    assert(chapter.GetType() == ChapterType::game_lost);
  }
  //Chapter 400: Game won
  {
    const Chapter chapter("../Files/400.txt");
    assert(chapter.GetType() == ChapterType::game_won);
  }
  //Chapter 8: change status
  {
    const Chapter chapter("../Files/8.txt");
    assert(chapter.GetAddItems().count(Item::golden_brooch));
    assert(chapter.GetChangeLuck() == 2);
  }
  //Chapter 26: no change of status
  {
    const Chapter chapter("../Files/26.txt");
    assert(chapter.GetNextChapter() == 296);
  }
  //Chapter 206: dice game
  {
    const Chapter chapter("../Files/206.txt");
    assert(chapter.GetType() == ChapterType::play_dice);
    assert(chapter.GetNextChapter() == 296);
    assert(!chapter.GetByeText().empty());
    Character character(100,100,100,Item::shield);
    chapter.Do(character,true);
  }
  //Chapter 223: pill game
  {
    const Chapter chapter("../Files/223.txt");
    assert(chapter.GetType() == ChapterType::play_pill);
    assert(chapter.GetNextChapter() == 165);
    assert(chapter.GetChangeGold() == 20);
    assert(!chapter.GetByeText().empty());
    Character character(100,100,100,Item::shield);
    chapter.Do(character,true);
  }
  //Chapter 378: ball game
  {
    const Chapter chapter("../Files/378.txt");
    assert(chapter.GetType() == ChapterType::play_ball);
    assert(chapter.GetNextChapter() == 52);
    assert(!chapter.GetByeText().empty());
    Character character(100,100,100,Item::shield);
    chapter.Do(character,true);
  }
  //Chapter 1: options
  {
    const Chapter chapter("../Files/1.txt");
    assert(chapter.GetType() == ChapterType::normal);
    assert(chapter.GetOptions().GetOptions().size() == 3);
    Character character(100,100,100,Item::shield);
    chapter.Do(character,true);
  }
  //Chapter 3: options
  {
    const Chapter chapter("../Files/3.txt");
    assert(chapter.GetType() == ChapterType::normal);
    assert(chapter.GetOptions().GetOptions().size() == 2);
    Character character(1,1,1,Item::shield);
    assert(chapter.GetOptions().GetValidOptions(character).size() == 2);
    character.ChangeGold(-character.GetGold()); //Make player bankrupt
    assert(chapter.GetOptions().GetValidOptions(character).size() == 1);
  }
  //Chapter 7: 2 options and status change
  {
    const Chapter chapter("../Files/7.txt");
    assert(chapter.GetOptions().GetOptions().size() == 2);
    Character character(10,10,10,Item::shield);
    character.ChangeLuck(-character.GetLuck()); //Make player unlucky
    assert(character.GetLuck() == 0);
    assert(!character.HasItem(Item::black_pearls));
    chapter.Do(character,true);
    assert(character.GetLuck() > 0);
    assert(character.HasItem(Item::black_pearls));
  }


  //Chapter 14: must respond to ring of fire
  {
    const Chapter chapter("../Files/14.txt");
    assert(chapter.GetOptions().GetOptions().size() == 2);
    Character character(1,1,1,Item::shield);
    assert(chapter.GetOptions().GetValidOptions(character).size() == 1);
    assert(chapter.GetOptions().GetValidOptions(character)[0].GetNextChapter() == 237);
    character.AddItem(Item::ring_of_fire);
    assert(chapter.GetOptions().GetValidOptions(character).size() == 1);
    assert(chapter.GetOptions().GetValidOptions(character)[0].GetNextChapter() == 191);
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

