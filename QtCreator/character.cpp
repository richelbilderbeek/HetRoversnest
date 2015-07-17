#include "character.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <fstream>

Character::Character(
  const int dexterity, //NL: Behendigheid
  const int condition, //NL: Conditie
  const int luck, //NL: Geluk
  const Item initial_item
)
  : m_chapters{std::vector<int>(1,1)},
    m_dexterity{dexterity},
    m_gold{30},
    m_initial_dexterity{dexterity},
    m_initial_luck{luck},
    m_initial_stamina{condition},
    m_items{},
    m_luck{luck},
    m_provisions{10},
    m_stamina{condition}
{
  m_items.insert(Item::shield);
  m_items.insert(Item::carralifs_sword);
  m_items.insert(initial_item);
}

void Character::AddItem(const Item item)
{
  m_items.insert(item);
}

int Character::CalcAttackStrength() const noexcept
{
  return m_dexterity
    + ((std::rand() >> 4) % 6)
    + ((std::rand() >> 4) % 6)
  ;
}

void Character::ChangeDexterity(const int change)
{
  m_dexterity += change;
  m_dexterity = std::min(m_dexterity,m_initial_dexterity);
}

void Character::ChangeGold(const int change)
{
  m_gold += change;
  assert(m_gold >= 0);
}

void Character::ChangeProvisions(const int change)
{
  m_provisions += change;
  //Provisions can become less than zero, due to chapter 42
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


int Character::GetDexterity() const noexcept
{
  return
    GetDexterityBase()
    + (this->HasItem(Item::shield) ? 1 : 0)
    + (this->HasItem(Item::chainmail_coat) ? 2 : 0)
  ;
}

int Character::GetDexterityBase() const noexcept
{
  return m_dexterity;
}

bool Character::HasItem(const Item item) const
{
  /*
  if (item == Item::something_silver)
  {
    return HasItem(Item::silver_arrow)
      || HasItem(Item::silver_brooch)
      || HasItem(Item::silver_chain)
      || HasItem(Item::silver_flute)
      || HasItem(Item::silver_goblet)
      || HasItem(Item::silver_insect_bracelet)
    ;
  }
  */
  return m_items.find(item) != std::end(m_items);
}

void Character::RemoveItem(Item item)
{
  if (m_items.empty()) return;
  if (item == Item::two_random_items)
  {
    const int n_items{static_cast<int>(m_items.size())};
    const int item_index{std::rand() % n_items};
    auto iter = m_items.cbegin();
    for (int i=0; i!=item_index; ++i) { ++iter; }
    assert(iter != m_items.cend());
    item = *iter;
  }
  m_items.erase(item);
}

void Character::SetChapter(const int chapter)
{
  if (
    std::find(std::begin(m_chapters),std::end(m_chapters),chapter) != std::end(m_chapters)
  )
  {
    std::cerr << "ERROR: entering same chapter twice!" << std::endl;
    std::ofstream f("Path.txt");
    const auto v = GetChapters();
    std::copy(std::begin(v),std::end(v),std::ostream_iterator<int>(f," "));
  }
  //assert(std::find(std::begin(m_chapters),std::end(m_chapters),chapter) == std::end(m_chapters));
  m_chapters.push_back(chapter);
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
