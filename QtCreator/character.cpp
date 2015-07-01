#include "character.h"

#include <cstdlib>

Character::Character(
  const int dexterity, //NL: Behendigheid
  const int condition, //NL: Conditie
  const int luck, //NL: Geluk
  const Item initial_item
)
  : m_dexterity{dexterity},
    m_initial_dexterity{dexterity},
    m_initial_luck{luck},
    m_initial_stamina{condition},
    m_items{},
    m_luck{luck},
    m_stamina{condition}
{
  m_items.insert(initial_item);
}

void Character::AddItem(const Item item)
{
  m_items.insert(item);
}

void Character::ChangeDexterity(const int change)
{
  m_dexterity += change;
  m_dexterity = std::min(m_dexterity,m_initial_dexterity);
}

void Character::ChangeStamina(const int change)
{
  m_stamina += change;
  m_stamina = std::min(m_stamina,m_initial_stamina);
}

void Character::ChangeLuck(const int change)
{
  m_luck += change;
  m_luck = std::min(m_luck,m_initial_luck);
}

bool Character::HasItem(const Item item)
{
  return m_items.find(item) != std::end(m_items);
}

void Character::RemoveItem(const Item item)
{
  m_items.erase(item);
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
