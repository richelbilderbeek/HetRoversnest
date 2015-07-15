#include "monster.h"
#include <cassert>
#include <iostream>

Monster::Monster(
  const std::string& name,
  const int dexterity,
  const int stamina,
  const int attack_strength
)
  : m_attack_strength{attack_strength},
    m_dexterity{dexterity},
    m_initial_stamina{stamina},
    m_name{name},
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

std::ostream& operator<<(std::ostream& os, const Monster& monster)
{
  os << monster.GetName() << " "
     << monster.GetDexterity() << " "
     << monster.GetStamina() << " "
     << monster.GetAttackStrength()
  ;
  return os;
}
