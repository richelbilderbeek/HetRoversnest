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
    assert(chapter.GetConsequence().GetItemsToAdd().size() == 1);
    assert(chapter.GetConsequence().GetItemsToAdd()[0] == Item::golden_brooch);
    assert(chapter.GetConsequence().GetChangeLuck() == 2);
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
    assert(chapter.GetConsequence().GetChangeGold() == 20);
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
    assert(chapter.GetConsequence().GetItemsToAdd().size() == 1);
    assert(chapter.GetConsequence().GetItemsToAdd()[0] == Item::black_pearls);
    assert(chapter.GetConsequence().GetItemsToRemove().empty());
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

  //Chapter 11: lose 1 skill point and shield
  {
    const Chapter chapter("../Files/11.txt");
    Character character(10,10,10,Item::luck_potion);
    assert(character.HasItem(Item::shield));
    const int dex_before{character.GetDexterity()};
    chapter.Do(character,true);
    const int dex_after{character.GetDexterity()};
    assert(dex_after == dex_before - 1); //Due to losing shield
    assert(!character.HasItem(Item::shield));
  }
  //Chapter 11: must lose 3 skill points and shield and chain mail
  {
    const Chapter chapter("../Files/11.txt");
    Character character(10,10,10,Item::shield);
    character.AddItem(Item::chainmail_coat);
    const int dex_before{character.GetDexterity()};
    assert(character.HasItem(Item::shield));
    assert(character.HasItem(Item::chainmail_coat));
    chapter.Do(character,true);
    assert(!character.HasItem(Item::shield));
    assert(!character.HasItem(Item::chainmail_coat));
    const int dex_after{character.GetDexterity()};
    assert(dex_after == dex_before - 3); //Due to losing shield and chainmail
  }
  //Chapter 19: 2 options and status change
  {
    const Chapter chapter("../Files/19.txt");
    assert(chapter.GetOptions().GetOptions().size() == 2);
    Character character(10,10,10,Item::shield);
    const int dex_before{character.GetDexterity()};
    const int sta_before{character.GetStamina()};
    chapter.Do(character,true);
    const int dex_after{character.GetDexterity()};
    const int sta_after{character.GetStamina()};
    assert(dex_after == dex_before - 1);
    assert(sta_after == sta_before - 4);
  }
  //Chapter 15: Luck chapter must be parsed correctly
  {
    const Chapter chapter("../Files/15.txt");
    assert(!chapter.GetLuck().GetLuckText().empty());
    assert(!chapter.GetLuck().GetNoLuckText().empty());
    assert(chapter.GetLuck().GetLuckChapter() > 1);
    assert(chapter.GetLuck().GetNoLuckChapter() > 1);
  }
  //Chapter 15: Luck chapter must be parsed correctly
  {
    const Chapter chapter("../Files/15.txt");
    Character character(10,10,10,Item::shield);
    chapter.Do(character,true);
    assert(character.GetCurrentChapter() > -1);
  }
  //Chapter 18: Skill chapter must be parsed correctly
  {
    const Chapter chapter("../Files/18.txt");
    assert(!chapter.GetSkill().GetSkillText().empty());
    assert(!chapter.GetSkill().GetNoSkillText().empty());
    assert(chapter.GetSkill().GetSkillChapter() > 1);
    assert(chapter.GetSkill().GetNoSkillChapter() > 1);
  }
  //Chapter 18: Skill chapter must be parsed correctly
  {
    const Chapter chapter("../Files/18.txt");
    Character character(10,10,10,Item::shield);
    chapter.Do(character,true);
    assert(character.GetCurrentChapter() > -1);
  }
  //Chapter 42: Must loose all provisions
  {
    const Chapter chapter("../Files/42.txt");
    Character character(10,10,10,Item::shield);
    assert(character.GetProvisions() > 0);
    chapter.Do(character,true);
    assert(character.GetProvisions() == 0);
  }

  //Chapter 53: Can only give if something silver
  if (1==2)
  {
    const Chapter chapter("../Files/53.txt");
    Character character(10,10,10,Item::shield);
    character.AddItem(Item::silver_arrow);
    chapter.Do(character,true);
    chapter.Do(character,true);
    chapter.Do(character,true);
    chapter.Do(character,true);
    assert(!character.HasItem(Item::silver_arrow));
  }

  //Chapter 63: Lose two random items
  {
    const Chapter chapter("../Files/63.txt");
    Character character(10,10,10,Item::shield);
    const int n_items_before{static_cast<int>(character.GetItems().size())};
    chapter.Do(character,true);
    const int n_items_after{static_cast<int>(character.GetItems().size())};
    assert(n_items_after == n_items_before - 2);
  }

  //Chapter 134: Lose all gold
  {
    const Chapter chapter("../Files/134.txt");
    Character character(10,10,10,Item::shield);
    assert(character.GetGold() > 0);
    chapter.Do(character,true);
    assert(character.GetGold() == 0);
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

