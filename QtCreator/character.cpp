#include "character.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <iterator>
#include <fstream>

#include "dice.h"
#include "helper.h"

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
  m_items.push_back(Item::shield);
  m_items.push_back(Item::carralifs_sword);
  assert(
    initial_item == Item::dexterity_potion
    || initial_item == Item::luck_potion
    || initial_item == Item::stamina_potion
  );
  m_items.push_back(initial_item);
}

void Character::AddHasFought(const std::string& monster_name)
{
  assert(!HasFought(monster_name));
  m_fought.push_back(monster_name);
}

void Character::AddItem(const Item item)
{
  if (HasItem(item))
  {
    #ifndef NDEBUG
    std::cerr << "WARNING: adding item " << item << " for the second time!" << std::endl;
    #endif
    ShowInventory(ShowTextMode::debug);
    assert(!"Should not get here");
  }
  m_items.push_back(item);
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
    + (Dice::Get()->Throw())
    + (Dice::Get()->Throw())
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
    //std::cerr << "WARNING: Character's gold became negative, set to zero instead" << std::endl;
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
  const int n{static_cast<int>(
      std::count(std::begin(m_items),std::end(m_items),item)
    )
  };
  assert(n == 0 || n == 1);
  return n;
}

void Character::RemoveItem(Item item)
{
  if (m_items.empty()) return;
  if (item == Item::two_random_items)
  {
    auto items = m_items;
    if (items.empty()) { return; }
    std::shuffle(std::begin(items),std::end(items),Dice::Get()->GetEngine());
    RemoveItem(items.front());
    if (items.empty()) { return; }
    std::shuffle(std::begin(items),std::end(items),Dice::Get()->GetEngine());
    RemoveItem(items.front());
  }
  else if (item == Item::all_silver_items)
  {
    const auto silver_items = {
      Item::silver_arrow,
      Item::silver_chalice,
      Item::silver_flute,
      Item::silver_insect_bracelet,
      Item::silver_scorpion_brooch,
      Item::silver_spoon
    };
    for (const auto silver_item: silver_items)
    {
      if (HasItem(silver_item)) RemoveItem(silver_item);
    }
    return;
  }

  const auto iter = std::find(std::begin(m_items),std::end(m_items),item);
  if (iter == std::end(m_items))
  {
    std::cerr << "Warning:: attempted to remove item " << ToStr(item) << " which is not possessed\n";
    assert(!"Should not get here");
    return;
  }
  assert(iter != std::end(m_items));
  std::swap(*iter,m_items.back());
  m_items.pop_back();
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
    if (
         chapter != 96
      && chapter != 65
      && chapter != 122
      && chapter != 197
      && chapter != 231
      && chapter != 232
      && chapter != 234
      && chapter != 284
      && chapter != 314
      && chapter != 319
      && chapter != 383
    )
    {
      #ifndef NDEBUG
      std::cerr << "WARNING: entering chapter " << chapter << " for the second time!" << std::endl;
      #endif
      ShowInventory(ShowTextMode::debug);

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

void Character::ShowInventory(const ShowTextMode text_mode)
{
  std::stringstream s;
  s
    << "dexterity:\n"
    << " * base: " << GetDexterityBase() << "/" << GetInitialDexterity() << '\n'
  ;

  //Shield
  if (HasItem(Item::shield_with_tower_crest))
  {
    s << " * " << ToPrettyStr(Item::shield_with_tower_crest) << ": -1 (equipped, cursed)\n";

    if (HasItem(Item::shield_with_unicorn_crest)) { s << " * " << ToPrettyStr(Item::shield_with_unicorn_crest) << ": +3\n"; }
    if (HasItem(Item::magnificent_shield)) { s << " * " << ToPrettyStr(Item::magnificent_shield) << ": +2\n"; }
    if (HasItem(Item::shield)) { s << " * " << ToPrettyStr(Item::shield) << ": +1\n"; }
  }
  else if (HasItem(Item::shield_with_unicorn_crest))
  {
    s << " * " << ToPrettyStr(Item::shield_with_unicorn_crest) << ": +3 (equipped)\n";
    if (HasItem(Item::magnificent_shield)) { s << " * " << ToPrettyStr(Item::magnificent_shield) << ": +2\n"; }
    if (HasItem(Item::shield)) { s << " * " << ToPrettyStr(Item::shield) << ": +1\n"; }
  }
  else if (HasItem(Item::magnificent_shield))
  {
    if (HasItem(Item::magnificent_shield)) { s << " * " << ToPrettyStr(Item::magnificent_shield) << ": +2 (equipped)\n"; }
    if (HasItem(Item::shield)) { s << " * " << ToPrettyStr(Item::shield) << ": +1\n"; }
  }
  else if (HasItem(Item::shield))
  {
    if (HasItem(Item::shield)) { s << " * " << ToPrettyStr(Item::shield) << ": +1 (equipped)\n"; }
  }

  if (HasItem(Item::carralifs_sword))
  {
    s << " * " << ToPrettyStr(Item::carralifs_sword) << ": +2 (equipped) \n";
    if (HasItem(Item::ordinary_sword)) { s << " * " << ToPrettyStr(Item::ordinary_sword) << ": +1\n"; }
  }
  else if (HasItem(Item::ordinary_sword))
  {
    if (HasItem(Item::ordinary_sword)) { s << " * " << ToPrettyStr(Item::ordinary_sword) << ": +1 (equipped)\n"; }
  }

  if (HasItem(Item::magic_elven_boots)) { s << " * " << ToPrettyStr(Item::magic_elven_boots) << ": +1\n"; }
  if (HasItem(Item::chainmail_coat)) { s << " * " << ToPrettyStr(Item::chainmail_coat) << ": +2\n"; }
  s
    << " * total: " << GetDexterity() << "/" << GetInitialDexterity() << '\n'
    << "stamina: " << GetStamina() << "/" << GetInitialStamina() << '\n'
    << "luck:\n"
    << " * base: " << GetLuckBase() << "/" << GetInitialLuck() << '\n'
  ;

  if (HasItem(Item::golden_scorpion_brooch)) { s << " * " << ToPrettyStr(Item::golden_scorpion_brooch) << ": +2\n"; }
  //Note: the copper brooch decreases luck with 1, but this is not shown on purpose
  //I cannot avoid that it will be easy to see that base luck and total luck don't match
  s
    << " * total: " << GetLuck() << "/" << GetInitialDexterity() << '\n'
    << "gold pieces: " << GetGold() << '\n'
    << "provisions: " << GetProvisions() << '\n'
    << '\n'
  ;

  if (GetArrows() != 0)
  {
    s << "You got " << GetArrows() << " sticking in your body\n";
  }

  s << "items: \n";
  for (const Item item: GetItems())
  {
    s << " * " << ToPrettyStr(item) << '\n';
  }

  s << "monsters fought: \n";

  for (const auto monster_name: m_fought)
  {
    s << " * " << monster_name << '\n';
  }

  ShowText(s.str(),text_mode);
}

bool Character::TestDexterity() noexcept
{
  const int dice{
      Dice::Get()->Throw()
    + Dice::Get()->Throw()
  };
  return dice < GetDexterity();
}

bool Character::TestLuck() noexcept
{
  const int dice{
      Dice::Get()->Throw()
    + Dice::Get()->Throw()
  };
  const bool has_luck{dice < GetLuck()};
  --m_luck;
  return has_luck;
}
