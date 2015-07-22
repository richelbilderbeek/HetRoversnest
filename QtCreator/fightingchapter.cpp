#include "fightingchapter.h"

#include <cassert>
#include <sstream>
#include <iostream>

#include <boost/lexical_cast.hpp>

#include "character.h"
#include "chapter.h"
#include "dice.h"
#include "helper.h"

FightingChapter::FightingChapter(Chapter& chapter)
  : m_chapter{chapter},
    m_escape_chapter{-1},
    m_fight_sequentially{true},
    m_monsters{},
    m_rounds_to_escape{1000}
{

}

void FightingChapter::AddMonster(const Monster& monster)
{
  m_monsters.push_back(monster);
}

void FightingChapter::Do(Character& character) const
{

  if (DoFightSequentially())
  {
    DoFight(GetMonsters(),character);
  }
  else
  {
    DoFightTwoMonsters(GetMonsters(),character);
  }
  if (character.HasItem(Item::silver_scorpion_brooch))
  {
    character.ChangeStamina(1);
  }
}

void FightingChapter::DoFight(std::vector<Monster> monsters, Character& character) const
{
  for (auto monster: monsters)
  {
    DoFight(monster,character);
    if (character.IsDead()) return;
  }
}

void FightingChapter::DoFightTwoMonsters(std::vector<Monster> monsters,Character& character) const
{
  //Fight both
  assert(monsters.size() == 2);
  for (int round=0; ; ++round)
  {
    if (character.IsDead()) { return; }
    if (monsters[0].IsDead())
    {
      m_chapter.m_signal_show_text("You defeated the " + monsters[0].GetName() + "!\n");
      character.AddHasFought(monsters[0].GetName());
      break;
    }

    {
      std::stringstream s;
      s
        << '\n'
        << "Fight round #" << round << '\n'
        << "You " << character.GetDexterity() << " "
        << character.GetStamina() << "/"
        << character.GetInitialStamina() << '\n'
        << monsters[0].GetName() << " "
        << monsters[0].GetDexterity() << " "
        << monsters[0].GetStamina() << "/"
        << monsters[0].GetInitialStamina() << '\n'
        << monsters[1].GetName() << " "
        << monsters[1].GetDexterity() << " "
        << monsters[1].GetStamina() << "/"
        << monsters[1].GetInitialStamina() << '\n'
        << "Fight round #" << round
        << '\n'
      ;
      m_chapter.m_signal_show_text(s.str());
    }

    {
      const int monster_attack{monsters[0].CalcAttackStrength()};
      const int player_attack{character.CalcAttackStrength()};
      if (player_attack > monster_attack)
      {
        const int damage{2};
        monsters[0].ChangeStamina(-damage);
        m_chapter.m_signal_show_text("You hit the " + monsters[0].GetName() + ".\n");
        m_chapter.m_signal_show_text("Do you want to use luck?\n");

        while (1)
        {
          m_chapter.m_signal_show_text("[0] No\n");
          m_chapter.m_signal_show_text("[1] Yes\n");
          const std::string input{*m_chapter.m_signal_request_input()};
          if (input != "0" && input != "1") { m_chapter.m_signal_show_text("Please enter 0 or 1"); }
          if (input == "1")
          {
            const bool has_luck{character.TestLuck()};
            monsters[0].ChangeStamina( (damage/2) * (has_luck ? -1 : 1) );
          }
        }
      }
      else if (player_attack < monster_attack)
      {
        const int damage{monsters[0].GetAttackDamage()};
        character.ChangeStamina(-damage);
        m_chapter.m_signal_show_text("You were hit by the " + monsters[0].GetName() + ".\n");
        m_chapter.m_signal_show_text("Do you want to use luck?\n");

        while (1)
        {
          m_chapter.m_signal_show_text("[0] No\n");
          m_chapter.m_signal_show_text("[1] Yes\n");
          const std::string input{*m_chapter.m_signal_request_input()};
          if (input != "0" && input != "1") { m_chapter.m_signal_show_text("Please enter 0 or 1"); }
          if (input == "1")
          {
            const bool has_luck{character.TestLuck()};
            character.ChangeStamina( (damage/2) * (has_luck ? 1 : -1) );
          }
        }
      }
      else
      {
        m_chapter.m_signal_show_text("No damage was dealt.\n");
      }
    }
    //Second monster
    {
      const int monster_attack{monsters[1].CalcAttackStrength()};
      const int player_attack{character.CalcAttackStrength()};
      if (player_attack >= monster_attack)
      {
        m_chapter.m_signal_show_text("You resisted the " + monsters[1].GetName() + ".\n");
      }
      else if (player_attack < monster_attack)
      {
        const int damage{monsters[1].GetAttackDamage()};
        character.ChangeStamina(-damage);
        m_chapter.m_signal_show_text("You were hit by the " + monsters[1].GetName() + ".\n");
        m_chapter.m_signal_show_text("Do you want to use luck?\n");

        while (1)
        {
          m_chapter.m_signal_show_text("[0] No\n");
          m_chapter.m_signal_show_text("[1] Yes\n");
          const std::string input{*m_chapter.m_signal_request_input()};
          if (input != "0" && input != "1") { m_chapter.m_signal_show_text("Please enter 0 or 1"); }
          if (input == "1")
          {
            const bool has_luck{character.TestLuck()};
            character.ChangeStamina( (damage/2) * (has_luck ? 1 : -1) );
          }
        }
      }
    }
    m_chapter.m_signal_wait();
  }

  //Fight the remaining monster normally
  DoFight(monsters[1],character);
}


void FightingChapter::DoFight(Monster monster,Character& character) const
{
  for (int round = 1; ; ++round)
  {
    if (character.IsDead()) break;
    if (monster.IsDead())
    {
      character.AddHasFought(monster.GetName());
      assert(character.HasFought(monster.GetName()));
      break;
    }

    {
      std::stringstream s;
      s
        << '\n'
        << "Fight round #" << round << '\n'
        << "You " << character.GetDexterity() << " "
        << character.GetStamina() << "/"
        << character.GetInitialStamina() << '\n'
        << monster.GetName() << " "
        << monster.GetDexterity() << " "
        << monster.GetStamina() << "/"
        << monster.GetInitialStamina() << '\n'
      ;
      m_chapter.m_signal_show_text(s.str());
    }

    const int monster_attack{monster.CalcAttackStrength()};
    const int player_attack{character.CalcAttackStrength()};

    {
      std::stringstream s;
      s
        << "You attack with strength " << player_attack << '\n'
        << monster.GetName() << " attacks with strength " << monster_attack << '\n'
      ;
      m_chapter.m_signal_show_text(s.str());
    }

    if (player_attack > monster_attack)
    {
      m_chapter.m_signal_show_text("You hit the " + monster.GetName() + ".\n");
      m_chapter.m_signal_show_text("Do you want to use luck?\n");
      while (1)
      {
        m_chapter.m_signal_show_text("[0] No\n");
        m_chapter.m_signal_show_text("[1] Yes\n");
        const std::string input{*m_chapter.m_signal_request_input()};
        if (input != "0" && input != "1") { m_chapter.m_signal_show_text("Please enter 0 or 1"); }
        if (input == "0")
        {
          const int damage{2};
          monster.ChangeStamina(-damage);
          m_chapter.m_signal_show_text("You did the " + monster.GetName()
            + " " + boost::lexical_cast<std::string>(damage) + " points of damage \n"
          );
          break;
        }
        else if (input == "1")
        {
          const bool has_luck{character.TestLuck()};
          const int damage{2 + ((damage/2) * (has_luck ? 1 : -1))};
          monster.ChangeStamina(-damage);
          m_chapter.m_signal_show_text("You did the " + monster.GetName()
            + " " + std::to_string(damage) + " points of damage \n"
          );
          break;
        }
      }
    }
    else if (player_attack < monster_attack)
    {
      m_chapter.m_signal_show_text("You were hit by the " + monster.GetName() + ".\n");
      m_chapter.m_signal_show_text("Do you want to use luck?\n");

      while (1)
      {
        m_chapter.m_signal_show_text("[0] No\n");
        m_chapter.m_signal_show_text("[1] Yes\n");
        const std::string input{*m_chapter.m_signal_request_input()};
        if (input != "0" && input != "1") { m_chapter.m_signal_show_text("Please enter 0 or 1"); }
        if (input == "0")
        {
          const int damage{monster.GetAttackDamage()};
          character.ChangeStamina(-damage);
          m_chapter.m_signal_show_text("The " + monster.GetName()
            + " did " + std::to_string(damage) + " points of damage \n"
          );
          break;
        }
        else if (input == "1")
        {
          const bool has_luck{character.TestLuck()};
          const int damage{
            monster.GetAttackDamage()
            + ( (monster.GetAttackDamage()/2) * (has_luck ? -1 : 1) )
          };
          character.ChangeStamina(-damage);
          m_chapter.m_signal_show_text("The " + monster.GetName()
            + " did " + std::to_string(damage) + " points of damage \n"
          );
          break;
        }
      }
    }
    else
    {
      m_chapter.m_signal_show_text("No damage was dealt.\n");
    }

    m_chapter.m_signal_wait();

    if (character.IsDead()) break;

    //Fire breath
    if (monster.HasFireBreath())
    {
      m_chapter.m_signal_show_text("The monster uses its fiery breath....\n");
      m_chapter.m_signal_wait();
      if (Dice::Get()->Throw() <= 3)
      {
        m_chapter.m_signal_show_text("The fire hits you!\n");
        m_chapter.m_signal_wait();
        character.ChangeStamina(-2);
      }
      else
      {
        m_chapter.m_signal_show_text("The fire missed you!\n");
        m_chapter.m_signal_wait();
      }
    }
  }

  if (character.IsDead())
  {
    m_chapter.m_signal_show_text("The " + monster.GetName() + " defeated you.\n");
    m_chapter.m_signal_wait();
  }
  else
  {
    m_chapter.m_signal_show_text("You defeated the " + monster.GetName() + "!\n");
    m_chapter.m_signal_wait();
  }
}

void FightingChapter::SetEscapeToChapter(const int escape_to_chapter)
{
  m_escape_chapter = escape_to_chapter;
  assert(m_escape_chapter > 0);

}

void FightingChapter::SetRoundsToEscape(const int rounds_to_escape)
{
  m_rounds_to_escape = rounds_to_escape;
  assert(m_rounds_to_escape >= 0);
}
