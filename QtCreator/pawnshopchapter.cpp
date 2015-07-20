#include "pawnshopchapter.h"

#include <cassert>
#include <iostream>

#include "character.h"
#include "helper.h"

PawnShopChapter::PawnShopChapter()
  : m_items{}
{
}

void PawnShopChapter::AddItem(const Item item, const int price)
{
  m_items.push_back(std::make_pair(item,price));
}

void PawnShopChapter::Do(Character& character, const bool auto_play) const
{
  //Must be a copy, as the copy can get cleared
  std::vector<std::pair<Item,int>> items = this->GetItems();

  while (1)
  {
    std::cout << "[0] Leave shop\n";
    const int n_items{static_cast<int>(items.size())};
    bool can_sell{false};
    for (int i=0; i!=n_items; ++i)
    {
      std::cout
        << '[' << (character.HasItem(items[i].first) ? std::to_string(i+1) : " ") << "] Sell "
        << ToStr(items[i].first) << " for "
        << items[i].second << " gold pieces\n"
      ;
      if (character.HasItem(items[i].first)) { can_sell = true; }
    }
    if (!can_sell)
    {
      std::cout << "You do not have any items to sell.\n";
      break;
    }
    if (auto_play)
    {
      for (int i=0; i < static_cast<int>(items.size()); ++i)
      {
        if (character.HasItem(items[i].first))
        {
          std::cout << "You sold " << ToStr(items[i].first) << std::endl;
          character.RemoveItem(items[i].first);
          character.ChangeGold(items[i].second);
          std::swap(items[i],items.back());
          items.pop_back();
          --i;
        }
      }
      break;
    }
    assert(!auto_play);

    //Pawn shop
    std::string s;
    std::getline(std::cin,s);
    if (s.empty()) continue;
    if (!IsInt(s))
    {
      std::cout << "Invalid command\n";
      continue;
    }
    const int command{std::stoi(s)};
    if (command == 0) break;
    const int i = command - 1;
    if (i < 0 || i >= static_cast<int>(items.size()))
    {
      std::cout << "Invalid number, choose zero to leave the shop or an item number to buy it.\n";
      continue;
    }
    assert(i >= 0);
    assert(i < static_cast<int>(items.size()));
    if (!character.HasItem(items[i].first))
    {
      std::cout << "You do not possess this item.\n";
      continue;
    }
    std::cout << "You sold " << ToStr(items[i].first) << std::endl;
    character.RemoveItem(items[i].first);
    character.ChangeGold(items[i].second);
    std::swap(items[i],items.back());
    items.pop_back();
  }
}

PawnShopChapter ParsePawnShopChapter(std::stringstream& s)
{
  PawnShopChapter c;
  while (1)
  {
    const std::string what{ReadString(s)};
    if (what == "@") break;
    assert(IsItem(what));
    const Item item{ToItem(what)};
    const int price{ReadInt(s)};
    c.AddItem(item,price);
  }
  return c;
}
