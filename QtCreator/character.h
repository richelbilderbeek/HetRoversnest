#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>
#include "item.h"

struct Character
{
  Character(
    const int dexterity, //NL: Behendigheid
    const int condition, //NL: Conditie
    const int luck, //NL: Geluk
    const Item initial_item
  );
  int GetCondition() const noexcept { return m_condition; }
  bool TestLuck() noexcept;

  private:
  int m_condition; //NL: Conditie
  int m_dexterity; //NL: Behendigheid
  const int m_initial_condition; //NL: Conditie
  const int m_initial_dexterity; //NL: Behendigheid
  const int m_initial_luck; //NL: Geluk
  std::vector<Item> m_items; //NL: Voorwerpen
  int m_luck; //NL: Geluk

};

#endif // CHARACTER_H
