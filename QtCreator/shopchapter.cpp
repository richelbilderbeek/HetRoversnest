#include "shopchapter.h"

#include <cassert>
#include <iostream>

#include "character.h"
#include "helper.h"

ShopChapter::ShopChapter()
  : m_items{}
{
}

void ShopChapter::AddItem(const Item item, const int price)
{
  m_items.push_back(std::make_pair(item,price));
}

void ShopChapter::Do(Character& character, const bool auto_play) const
{
  //Must be a copy, as the copy can get cleared
  std::vector<std::pair<Item,int>> items = this->GetItems();

  while (1)
  {
    if (items.empty())
    {
      std::cout << "There are no more items to buy.\n";
      break;
    }
    std::cout << "[0] Leave shop\n";
    const int n_items{static_cast<int>(items.size())};
    bool can_buy{false};
    for (int i=0; i!=n_items; ++i)
    {
      std::cout << '[' << (i+1) << "] Buy "
        << ToStr(items[i].first) << " for "
        << items[i].second << " gold pieces\n"
      ;
      if (items[i].second <= character.GetGold()) { can_buy = true; }
    }
    if (!can_buy)
    {
      std::cout << "You do not have enough money to buy anything.\n";
      break;
    }
    if (auto_play)
    {
      for (int i=0; i < static_cast<int>(items.size()); ++i)
      {
        if (items[i].second <= character.GetGold())
        {
          std::cout << "You bough " << ToStr(items[i].first) << std::endl;
          character.AddItem(items[i].first);
          character.ChangeGold(-items[i].second);
          std::swap(items[i],items.back());
          items.pop_back();
          --i;
        }
      }
      break;
    }
    assert(!auto_play);

    //Shop
    std::string s;
    std::getline(std::cin,s);
    if (s.empty()) continue;
    if (!IsInt(s))
    {
      std::cout << "Invalid command\n";
      continue;
    }
    const int command = std::stoi(s);
    if (command == 0) break;
    const int i = command - 1;
    if (i < 0 || i >= static_cast<int>(items.size()))
    {
      std::cout << "Invalid number, choose zero to leave the shop or an item number to buy it.\n";
      continue;
    }
    assert(i >= 0);
    assert(i < static_cast<int>(items.size()));
    if (character.GetGold() < items[i].second)
    {
      std::cout << "Cannot buy this item: not enough gold\n";
      continue;
    }
    std::cout << "You bough " << ToStr(items[i].first) << std::endl;
    character.AddItem(items[i].first);
    character.ChangeGold(-items[i].second);
    std::swap(items[i],items.back());
    items.pop_back();
  }
}

ShopChapter ParseShopChapter(std::stringstream& s)
{
  ShopChapter c;
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
