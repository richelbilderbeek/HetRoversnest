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
  const std::string monster_name_0{ToPretty(monsters[0].GetName())};
  const std::string monster_name_1{ToPretty(monsters[1].GetName())};
  for (int round=0; ; ++round)
  {
    if (character.IsDead()) { return; }
    if (monsters[0].IsDead())
    {
      m_chapter.m_signal_show_text("You defeated the " + monster_name_0 + "!\n");
      character.AddHasFought(monsters[0].GetName()); //Must be raw name
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
        << monster_name_0 << " "
        << monsters[0].GetDexterity() << " "
        << monsters[0].GetStamina() << "/"
        << monsters[0].GetInitialStamina() << '\n'
        << monster_name_1 << " "
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
        m_chapter.m_signal_show_text("You hit the " + monster_name_0 + ".\n");
        m_chapter.m_signal_show_text("Do you want to use luck?\n");

        while (1)
        {
          m_chapter.m_signal_show_text("[0] No\n");
          m_chapter.m_signal_show_text("[1] Yes\n");
          //const std::vector<int> user_options = {1,2};
          const int input{*m_chapter.m_signal_request_input( {0,1} )};
          int damage = 2;
          if (input == 1)
          {
            const bool has_luck{character.TestLuck()};
            damage += (has_luck ? -1 : 1);
          }
          monsters[0].ChangeStamina(-damage);
          m_chapter.m_signal_show_text(
            "You did the " + monster_name_0
            + " " + std::to_string(damage) + " points of damage \n"
          );
          break;
        }
      }
      else if (player_attack < monster_attack)
      {
        m_chapter.m_signal_show_text("You were hit by the " + monster_name_0 + ".\n");
        m_chapter.m_signal_show_text("Do you want to use luck?\n");

        while (1)
        {
          m_chapter.m_signal_show_text("[0] No\n");
          m_chapter.m_signal_show_text("[1] Yes\n");
          const int input{*m_chapter.m_signal_request_input( {0,1} )};
          int damage{monsters[0].GetAttackDamage()};
          if (input == 1)
          {
            const bool has_luck{character.TestLuck()};
            damage += ( (damage/2) * (has_luck ? 1 : -1) );
          }
          character.ChangeStamina(-damage);
          m_chapter.m_signal_show_text(
            "The " + monster_name_0
            + " hit you with " + std::to_string(damage) + " points of damage \n"
          );
          break;
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
        m_chapter.m_signal_show_text("You resisted the " + monster_name_1 + ".\n");
      }
      else if (player_attack < monster_attack)
      {
        m_chapter.m_signal_show_text("You were hit by the " + monster_name_1 + ".\n");
        m_chapter.m_signal_show_text("Do you want to use luck?\n");

        while (1)
        {
          m_chapter.m_signal_show_text("[0] No\n");
          m_chapter.m_signal_show_text("[1] Yes\n");
          const int input{*m_chapter.m_signal_request_input( {0,1} )};
          int damage{monsters[1].GetAttackDamage()};
          if (input == 1)
          {
            const bool has_luck{character.TestLuck()};
            damage += ( (damage/2) * (has_luck ? -1 : 1) );
          }
          character.ChangeStamina(-damage);
          m_chapter.m_signal_show_text(
            "The " + monster_name_1
            + " hit you with " + std::to_string(damage) + " points of damage \n"
          );
          break;
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
  const std::string monster_name{ToPretty(monster.GetName())};

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
        << monster_name << " "
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
        << monster_name << " attacks with strength " << monster_attack << '\n'
      ;
      m_chapter.m_signal_show_text(s.str());
    }

    if (player_attack > monster_attack)
    {
      m_chapter.m_signal_show_text("You hit the " + monster_name + ".\n");
      m_chapter.m_signal_show_text("Do you want to use luck?\n");
      while (1)
      {
        m_chapter.m_signal_show_text("[0] No\n");
        m_chapter.m_signal_show_text("[1] Yes\n");
        assert(m_chapter.m_signal_request_input.num_slots() > 0);
        const int input{*m_chapter.m_signal_request_input( {0,1} )};
        int damage{2};
        if (input == 1)
        {
          const bool has_luck{character.TestLuck()};
          damage += has_luck ? 1 : -1;
        }
        monster.ChangeStamina(-damage);
        m_chapter.m_signal_show_text("You did the " + monster_name
          + " " + boost::lexical_cast<std::string>(damage) + " points of damage \n"
        );
        break;
      }
    }
    else if (player_attack < monster_attack)
    {
      m_chapter.m_signal_show_text("You were hit by the " + monster_name + ".\n");
      m_chapter.m_signal_show_text("Do you want to use luck?\n");

      while (1)
      {
        m_chapter.m_signal_show_text("[0] No\n");
        m_chapter.m_signal_show_text("[1] Yes\n");
        const int input{*m_chapter.m_signal_request_input( {0,1} )};
        int damage{monster.GetAttackDamage()};
        if (input == 1)
        {
          const bool has_luck{character.TestLuck()};
          damage += ( (damage / 2 ) * (has_luck ? -1 : 1) );
        };
        character.ChangeStamina(-damage);
        m_chapter.m_signal_show_text("The " + monster_name
          + " did " + std::to_string(damage) + " points of damage \n"
        );
        break;
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
    m_chapter.m_signal_show_text("The " + monster_name + " defeated you.\n");
    m_chapter.m_signal_wait();
  }
  else
  {
    m_chapter.m_signal_show_text("You defeated the " + monster_name + "!\n");
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
