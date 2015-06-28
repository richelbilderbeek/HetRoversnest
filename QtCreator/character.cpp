#include "character.h"

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
