#include "monster.h"
#include <cassert>

Monster::Monster(
  const std::string& name,
  const int dexterity,
  const int stamina
)
  : m_dexterity{dexterity},
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
