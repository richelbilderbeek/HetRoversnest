#include "monster.h"

#include <algorithm>
#include <cassert>
#include <iostream>

#include "helper.h"

Monster::Monster(
  const std::string& name,
  const int dexterity,
  const int stamina,
  const int attack_damage,
  const bool has_fire_breath
)
  : m_attack_damage{attack_damage},
    m_dexterity{dexterity},
    m_has_fire_breath{has_fire_breath},
    m_initial_stamina{stamina},
    m_name{ToPretty(name)},
    m_stamina{stamina}
{
  assert(!name.empty());
  assert(dexterity > 0);
  assert(stamina > 0);
  assert(!IsDead());
}

void Monster::ChangeStamina(const int delta_stamina) noexcept
{
  assert(!IsDead());
  m_stamina += delta_stamina;
}

int Monster::CalcAttackStrength() const noexcept
{
  return m_dexterity
    + ((std::rand() >> 4) % 6)
    + ((std::rand() >> 4) % 6)
  ;
}

Monster ParseMonster(std::stringstream& s)
{
  const std::string name{ReadString(s)};
  const int dexterity{ReadInt(s)};
  const int condition{ReadInt(s)};
  const std::string attack{ReadString(s)};
  int attack_strength{2};
  bool has_fire_breath{false};
  if (IsInt(attack))
  {
    attack_strength = std::stoi(attack);
  }
  else
  {
    assert(attack == "2+fire");
    has_fire_breath = true;
  }
  const Monster monster(name,dexterity,condition,attack_strength,has_fire_breath);
  return monster;
}

std::vector<Monster> ParseMonsters(std::stringstream& s)
{
  std::vector<Monster> monsters;
  while (1)
  {
    const std::string what{ReadString(s)};
    if (what == "@") break;
    assert(what == "monster");
    const Monster monster{ParseMonster(s)};
    monsters.push_back(monster);
  }
  return monsters;
}

std::ostream& operator<<(std::ostream& os, const Monster& monster)
{
  os << monster.GetName() << " "
     << monster.GetDexterity() << " "
     << monster.GetStamina() << " "
     << monster.GetAttackDamage()
  ;
  return os;
}
