#ifndef CHARACTER_H
#define CHARACTER_H

#include <set>
#include "item.h"

struct Character
{
  Character(
    const int dexterity, //NL: Behendigheid
    const int condition, //NL: Conditie
    const int luck, //NL: Geluk
    const Item initial_item
  );

  void AddItem(const Item item);
  void ChangeDexterity(const int change);
  void ChangeStamina(const int change);
  void ChangeLuck(const int change);
  bool HasItem(const Item item);
  bool IsDead() const noexcept { return m_stamina <= 0; }
  void RemoveItem(const Item item);
  int GetStamina() const noexcept { return m_stamina; }
  bool TestDexterity() noexcept;
  bool TestLuck() noexcept;

  private:
  int m_dexterity; //NL: Behendigheid
  const int m_initial_dexterity; //NL: Behendigheid
  const int m_initial_luck; //NL: Geluk
  const int m_initial_stamina; //NL: Conditie
  std::set<Item> m_items; //NL: Voorwerpen
  int m_luck; //NL: Geluk
  int m_stamina; //NL: Conditie
};

#endif // CHARACTER_H
