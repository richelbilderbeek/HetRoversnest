#include "character.h"

#include <cstdlib>

Character::Character(
  const int dexterity, //NL: Behendigheid
  const int condition, //NL: Conditie
  const int luck, //NL: Geluk
  const Item initial_item
)
  : m_condition{condition},
    m_dexterity{dexterity},
    m_initial_condition{condition},
    m_initial_dexterity{dexterity},
    m_initial_luck{luck},
    m_items{},
    m_luck{luck}
{
  m_items.push_back(initial_item);
}

bool Character::TestDexterity() noexcept
{
  return (std::rand() >> 4) % 2;
}

bool Character::TestLuck() noexcept
{
  --m_luck;
  return (std::rand() >> 4) % 2;
}
