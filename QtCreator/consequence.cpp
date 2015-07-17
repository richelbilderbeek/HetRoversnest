#include "consequence.h"

#include <cassert>
#include <iostream>

#include "character.h"

Consequence::Consequence()
  :
    m_add_items{},
    m_change_dex{0},
    m_change_gold{0},
    m_change_luck{0},
    m_remove_items{},
    m_change_sta{0}
{
}

void Consequence::AddItemToAdd(const Item& item)
{
  m_add_items.push_back(item);
}

void Consequence::AddItemToRemove(const Item& item)
{
  m_remove_items.push_back(item);
}

void Consequence::Apply(Character& character) const
{
  const bool verbose{true};
  //Dex
  {
    const int change_dex{GetChangeDexterity()};
    if (change_dex != 0)
    {
      if (verbose) { std::clog << "Changing dexterity by " << change_dex << std::endl; }
      character.ChangeDexterity(change_dex);
    }
  }
  //Gold
  {
    const int change_gold{GetChangeGold()};
    if (change_gold != 0)
    {
      if (verbose) { std::clog << "Changing gold by " << change_gold << std::endl; }
      character.ChangeGold(change_gold);
    }
  }
  //Luck
  {
    const int change_luck{GetChangeLuck()};
    if (change_luck != 0)
    {
      if (verbose) { std::clog << "Changing luck by " << change_luck << std::endl; }
      character.ChangeLuck(change_luck);
    }
  }
  //Stamina
  {
    const int change_sta{GetChangeStamina()};
    if (change_sta != 0)
    {
      if (verbose) { std::clog << "Changing stamina by " << change_sta << std::endl; }
      character.ChangeStamina(change_sta);
    }
  }
  for (const auto item: this->GetItemsToAdd())
  {
    if (verbose) { std::clog << "Adding item " << ToStr(item) << std::endl; }
    character.AddItem(item);
  }
  for (const auto item: this->GetItemsToRemove())
  {
    if (item == Item::all_provisions)
    {
      if (verbose) { std::clog << "Removing all provisions" << std::endl; }
      character.ChangeProvisions(-character.GetProvisions());
      continue;
    }
    if (item == Item::two_random_items)
    {
      if (verbose) { std::clog << "Removing two random items" << std::endl; }
      for (int i=0; i!=2; ++i)
      {
        const auto items = character.GetItems();
        const int n_items{static_cast<int>(items.size())};
        if (n_items == 0) continue;
        auto iter = items.cbegin();
        const int item_index{(std::rand() >> 4) % n_items};
        for (int j=0; j!=item_index; ++j) { ++iter; }
        character.RemoveItem(*iter);
      }
      continue;
    }
    if (verbose) { std::clog << "Removing item " << ToStr(item) << std::endl; }
    character.RemoveItem(item);
  }
}
