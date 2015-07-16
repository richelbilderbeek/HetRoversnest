#include "monster.h"

#include <algorithm>
#include <cassert>
#include <iostream>

std::string ToPretty(std::string s)
{
  assert(!s.empty());
  std::replace(
    std::begin(s),
    std::end(s),
    '_',' '
  );
  s[0] = std::tolower(s[0]);
  return s;
}

Monster::Monster(
  const std::string& name,
  const int dexterity,
  const int stamina,
  const int attack_damage
)
  : m_attack_damage{attack_damage},
    m_dexterity{dexterity},
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

std::ostream& operator<<(std::ostream& os, const Monster& monster)
{
  os << monster.GetName() << " "
     << monster.GetDexterity() << " "
     << monster.GetStamina() << " "
     << monster.GetAttackDamage()
  ;
  return os;
}
