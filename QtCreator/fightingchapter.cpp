#include "fightingchapter.h"

#include <cassert>
#include <sstream>
#include <iostream>

#include "character.h"
#include "dice.h"
#include "helper.h"

FightingChapter::FightingChapter()
  : m_escape_chapter{-1},
    m_fight_sequentially{true},
    m_monsters{},
    m_rounds_to_escape{1000}
{

}

void FightingChapter::AddMonster(const Monster& monster)
{
  m_monsters.push_back(monster);
}

void FightingChapter::Do(Character& character, const bool auto_play) const
{

  if (DoFightSequentially())
  {
    DoFight(GetMonsters(),character,auto_play);
  }
  else
  {
    DoFightTwoMonsters(GetMonsters(),character,auto_play);
  }
  if (character.HasItem(Item::silver_scorpion_brooch))
  {
    character.ChangeStamina(1);
  }
}

void DoFight(
  std::vector<Monster> monsters,
  Character& character,
  const bool auto_play
)
{
  for (auto monster: monsters)
  {
    DoFight(monster,character,auto_play);
    if (character.IsDead()) return;
  }
}

void DoFightTwoMonsters(
  std::vector<Monster> monsters,
  Character& character,
  const bool auto_play
)
{
  //Fight both
  assert(monsters.size() == 2);
  for (int round=0; ; ++round)
  {

    if (character.IsDead()) { return; }
    if (monsters[0].IsDead())
    {
      character.AddHasFought(monsters[0].GetName());
      assert(character.HasFought(monsters[0].GetName()));
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
      ShowText(s.str(),auto_play);
    }

    {
      const int monster_attack{monsters[0].CalcAttackStrength()};
      const int player_attack{character.CalcAttackStrength()};
      if (player_attack > monster_attack)
      {
        const int damage{2};
        {
          std::stringstream s;
          s << "You hit the " << monsters[0].GetName() << ".\n";
          ShowText(s.str(),auto_play);
        }
        if (!auto_play)
        {
          //std::cout << "Do you want to use luck?" << std::endl;
          //assert(!"TODO");
          monsters[0].ChangeStamina(-damage);
        }
        else
        {
          monsters[0].ChangeStamina(-damage);
        }
      }
      else if (player_attack < monster_attack)
      {
        const int damage{monsters[0].GetAttackDamage()};
        {
          std::stringstream s;
          s << "You were hit by the " << monsters[0].GetName() << "\n.";
          ShowText(s.str(),auto_play);
        }
        if (!auto_play)
        {
          //std::cout << "Do you want to use luck?" << std::endl;
          //assert(!"TODO");
          character.ChangeStamina(-damage);
        }
        else
        {
          character.ChangeStamina(-damage);
        }
      }
      else
      {
        std::stringstream s;
        s << "No damage was dealt.\n";
        ShowText(s.str(),auto_play);
      }
    }
    //Second monster
    {
      const int monster_attack{monsters[1].CalcAttackStrength()};
      const int player_attack{character.CalcAttackStrength()};
      if (player_attack >= monster_attack)
      {
        std::stringstream s;
        s << "You resisted the " << monsters[1].GetName() << ".\n";
        ShowText(s.str(),auto_play);
      }
      else if (player_attack < monster_attack)
      {
        const int damage{monsters[1].GetAttackDamage()};
        {
          std::stringstream s;
          s<< "You were hit by the " << monsters[1].GetName() << "\n.";
          ShowText(s.str(),auto_play);
        }
        if (!auto_play)
        {
          //std::cout << "Do you want to use luck?" << std::endl;
          //assert(!"TODO");
          character.ChangeStamina(-damage);
        }
        else
        {
          character.ChangeStamina(-damage);
        }
      }
    }
    if (!auto_play) { Wait(1.0); }
  }
  {
    std::stringstream s;
    s << "You defeated the " << monsters[0].GetName() << "!\n";
    ShowText(s.str(),auto_play);
  }
  //character.AddHasFought(monsters[0].GetName());

  //Fight the remaining monster normally
  DoFight(monsters[1],character,auto_play);
  if (character.IsDead()) return;
}


void DoFight(
  Monster monster,
  Character& character,
  const bool auto_play
)
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
      ShowText(s.str(),auto_play);
    }

    const int monster_attack{monster.CalcAttackStrength()};
    const int player_attack{character.CalcAttackStrength()};

    {
      std::stringstream s;
      s
        << "You attack with strength " << player_attack << '\n'
        << monster.GetName() << " attacks with strength " << monster_attack << '\n'
      ;
      ShowText(s.str(),auto_play);
    }

    if (player_attack > monster_attack)
    {
      {
        std::stringstream s;
        s << "You hit the " << monster.GetName() << "." << '\n';
        ShowText(s.str(),auto_play);
      }
      const int damage = 2;
      if (!auto_play)
      {
        //std::cout
        //  << "Do you want to use luck? [1] Yes [2] No\n"
        //  << std::endl
        //;
        //assert(!"TODO");
        monster.ChangeStamina(-damage);
      }
      else
      {
        monster.ChangeStamina(-damage);
      }
    }
    else if (player_attack < monster_attack)
    {
      {
        std::stringstream s;
        s << "You were hit by the " << monster.GetName() << "." << '\n';
        ShowText(s.str(),auto_play);
      }
      const int damage{monster.GetAttackDamage()};
      if (!auto_play)
      {
        //std::cout << "Do you want to use luck?" << std::endl;
        //assert(!"TODO");
        character.ChangeStamina(-damage);
      }
      else
      {
        character.ChangeStamina(-damage);
      }
    }
    else
    {
      std::stringstream s;
      s << "No damage was dealt.\n";
      ShowText(s.str(),auto_play);
    }
    if (!auto_play) { Wait(1.0); }

    if (character.IsDead()) break;

    //Fire breath
    if (monster.HasFireBreath())
    {
      {
        std::stringstream s;
        s << "The monster uses its fiery breath....\n";
        ShowText(s.str(),auto_play);
        if (!auto_play) { Wait(1.0); }
      }
      if (Dice::Get()->Throw() <= 3)
      {
        std::stringstream s;
        s << "The fire hits you!\n";
        ShowText(s.str(),auto_play);
        if (!auto_play) { Wait(1.0); }
        character.ChangeStamina(-2);
      }
      else
      {
        std::stringstream s;
        s << "The fire missed you!\n";
        ShowText(s.str(),auto_play);
        if (!auto_play) { Wait(1.0); }
      }
    }
  }

  if (character.IsDead())
  {
    std::stringstream s;
    s << "\nThe " << monster.GetName() << " defeated you.\n";
    ShowText(s.str(),auto_play);
    if (!auto_play) { Wait(1.0); }
  }
  else
  {
    std::stringstream s;
    s << "\nYou defeated the " << monster.GetName() << "!\n";
    ShowText(s.str(),auto_play);
    if (!auto_play) { Wait(1.0); }
  }
}

void DoFightWithTime(std::stringstream& s, int& chapter, Character& character, const bool auto_play)
{
  assert(1==2);
  //TODO: Should be called
  const bool verbose{false};
  Parse(s,'@');

  //Name monster
  std::string name;
  {
    while (1)
    {
      char c = '*';
      s >> c;
      assert(c != '*');
      if (c == ' ' || c == '\n') continue;
      if (c == '@') break;
      name += c;
    }
  }
  if (verbose) { std::cout << "Monster name: " << name << std::endl; }
  //Skill monster
  int monster_dexterity = -1;
  {
    s >> monster_dexterity;
    assert(monster_dexterity != -1);
  }
  if (verbose) { std::cout << "Monster dexterity: " << monster_dexterity << std::endl; }
  //Condition monster
  int monster_stamina = -1;
  {
    Parse(s,'@');
    s >> monster_stamina;
    assert(monster_stamina != -1);
  }
  if (verbose) { std::cout << "Monster stamina: " << monster_stamina << std::endl; }
  //Number of rounds
  int number_of_rounds = -1;
  {
    Parse(s,'@');
    s >> number_of_rounds;
    assert(number_of_rounds > -1);
  }
  if (verbose) { std::cout << "Number of rounds: " << number_of_rounds << std::endl; }
  //New chapter after time limit
  int new_chapter_after_time_limit = -1;
  {
    Parse(s,'@');
    s >> new_chapter_after_time_limit;
    assert(new_chapter_after_time_limit > -1);
  }
  if (verbose) { std::cout << "New chapter after time limit: " << new_chapter_after_time_limit << std::endl; }
  //New chapter after time limit
  int new_chapter_within_time_limit = -1;
  {
    Parse(s,'@');
    s >> new_chapter_within_time_limit;
    assert(new_chapter_within_time_limit > -1);
  }
  if (verbose) { std::cout << "New chapter within time limit: " << new_chapter_within_time_limit << std::endl; }
  for (int i=0; i!=number_of_rounds; ++i)
  {
    const int monster_attack = Dice::Get()->Throw() + Dice::Get()->Throw();
    const int player_attack = Dice::Get()->Throw() + Dice::Get()->Throw();
    if (player_attack > monster_attack)
    {
      std::cout << "You hit the monster." << std::endl;
      if (!auto_play)
      {
        std::cout << "Do you want to use luck?" << std::endl;
        assert(!"TODO");
      }
      else
      {
        monster_stamina -= 2;
      }
    }
    else if (player_attack < monster_attack)
    {
      std::cout << "You were hit by the monster." << std::endl;
      if (!auto_play)
      {
        std::cout << "Do you want to use luck?" << std::endl;
        assert(!"TODO");
      }
      else
      {
        character.ChangeStamina(-2);
      }
    }
    else
    {
      std::cout << "No damage was dealt." << std::endl;
    }
    if (character.GetStamina() < 1)
    {

      std::cout
        << "The monster defeated you.\n"
        << "\n";
      return;
    }
    else if (monster_stamina < 1)
    {
      std::cout << "You defeated the monster." << std::endl;
      chapter = new_chapter_within_time_limit;
      return;
    }
  }
  std::cout << "You did not make it within the time limit." << std::endl;
  chapter = new_chapter_after_time_limit;
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
