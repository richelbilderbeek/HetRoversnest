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
    m_arrows{0},
    m_dexterity{dexterity},
    m_fought{},
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
  assert(
    initial_item == Item::dexterity_potion
    || initial_item == Item::luck_potion
    || initial_item == Item::stamina_potion
  );
  m_items.insert(initial_item);
}

void Character::AddHasFought(const std::string& monster_name)
{
  m_fought.push_back(monster_name);
}

void Character::AddItem(const Item item)
{
  m_items.insert(item);
}

int Character::CalcAttackStrength() const noexcept
{
  int shield_value = 0;
  if (HasItem(Item::shield_with_tower_crest)) { shield_value = 0; } //Cursed
  else if (HasItem(Item::shield_with_unicorn_crest)) { shield_value = 1; }
  else if (HasItem(Item::magnificent_shield)) { shield_value = 1; }
  else if (HasItem(Item::shield)) { shield_value = 0; }

  return GetDexterity()
    + (HasItem(Item::magic_helmet) ? 1 : 0)
    + shield_value
    + ((std::rand() >> 4) % 6)
    + ((std::rand() >> 4) % 6)
  ;
}

void Character::ChangeArrows(const int change)
{
  m_arrows += change;
  if (m_arrows < 0)
  {
    std::cerr << "WARNING: Character's number of arrows in his/her body is negative" << std::endl;
    assert(m_arrows >= 0);
  }
}


void Character::ChangeDexterity(const int change)
{
  m_dexterity += change;
  m_dexterity = std::min(m_dexterity,m_initial_dexterity);
}

void Character::ChangeGold(const int change)
{
  m_gold += change;
  if (m_gold < 0)
  {
    #ifndef NDEBUG
    std::cerr << "WARNING: Character's gold became negative, set to zero instead" << std::endl;
    #endif
    m_gold = 0;
    //assert(m_gold >= 0);
  }
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
  if (m_stamina < 0)
  {
    m_stamina = 0;
    this->SetIsDead();
  }
}

void Character::ChangeLuck(const int change)
{
  m_luck += change;
  m_luck = std::min(m_luck,m_initial_luck);
}

int Character::GetDexterity() const noexcept
{
  int shield_value = 0;
  if (HasItem(Item::shield_with_tower_crest)) { shield_value = -1; } //Cursed
  else if (HasItem(Item::shield_with_unicorn_crest)) { shield_value = 3; }
  else if (HasItem(Item::magnificent_shield)) { shield_value = 2; }
  else if (HasItem(Item::shield)) { shield_value = 1; }


  int sword_value = 0;
  if (HasItem(Item::ordinary_sword)) { sword_value = 1; }
  else if (HasItem(Item::carralifs_sword)) { sword_value = 2; }

  return
    GetDexterityBase()
    + (this->HasItem(Item::magic_elven_boots) ? 1 : 0)
    + shield_value
    + (this->HasItem(Item::chainmail_coat) ? 2 : 0)
    + sword_value
  ;
}

int Character::GetDexterityBase() const noexcept
{
  return m_dexterity;
}

int Character::GetLuck() const noexcept
{
  return
    GetLuckBase()
    + (this->HasItem(Item::copper_scorpion_brooch) ? -1 : 0)
    + (this->HasItem(Item::golden_scorpion_brooch) ? 2 : 0)
  ;
}

int Character::GetLuckBase() const noexcept
{
  return m_luck;
}

bool Character::HasFought(const std::string& monster_name) const
{
  return std::count(
    std::begin(m_fought),
    std::end(m_fought),
    monster_name
  ) > 0;
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
  if (chapter == -1)
  {
    std::cerr << "ERROR: cannot go to chapter -1" << std::endl;
    assert(!"Should not get here");
  }
  if (
    std::find(std::begin(m_chapters),std::end(m_chapters),chapter) != std::end(m_chapters)
  )
  {
    //These chapters can be legally visited more often
    if (chapter != 96
      && chapter != 197
      && chapter != 207
      && chapter != 234
      && chapter != 231
      && chapter != 314
    )
    {
      #ifndef NDEBUG
      std::cerr << "WARNING: entering chapter " << chapter << " for the second time!" << std::endl;
      #endif
      std::ofstream f("Path.txt");
      const auto v = GetChapters();
      std::copy(std::begin(v),std::end(v),std::ostream_iterator<int>(f," "));
      f.close();
      assert(!"Time to inspect path.txt");
    }
  }

  //assert(std::find(std::begin(m_chapters),std::end(m_chapters),chapter) == std::end(m_chapters));
  m_chapters.push_back(chapter);
}

bool Character::TestDexterity() noexcept
{
  const int dice{
      1 + ((std::rand() >> 4) % 6)
    + 1 + ((std::rand() >> 4) % 6)
  };
  return dice < GetDexterity();
}

bool Character::TestLuck() noexcept
{
  const int dice{
      1 + ((std::rand() >> 4) % 6)
    + 1 + ((std::rand() >> 4) % 6)
  };
  const bool has_luck{dice < GetLuck()};
  --m_luck;
  return has_luck;
}
