#include "shopchapter.h"

#include <cassert>
#include <iostream>
#include <sstream>

#include "chapter.h"
#include "character.h"
#include "helper.h"

ShopChapter::ShopChapter(Chapter * const chapter)
  : m_chapter{chapter}, m_items{}
{
}

void ShopChapter::AddItem(const Item item, const int price)
{
  m_items.push_back(std::make_pair(item,price));
}

void ShopChapter::Do(Character& character) const
{
  //Must be a copy, as the copy can get cleared
  std::vector<std::pair<Item,int>> items = this->GetItems();

  while (1)
  {
    if (items.empty())
    {
      m_chapter->m_signal_show_text("There are no more items to buy.\n");
      break;
    }
    std::vector<int> user_inputs;

    m_chapter->m_signal_show_text("[0] Leave shop\n");
    user_inputs.push_back(0);

    const int n_items{static_cast<int>(items.size())};
    for (int i=0; i!=n_items; ++i)
    {
      const auto item = items[i];
      std::stringstream s;
      s << '['
        << (item.second <= character.GetGold() ? std::to_string(i+1) : " ")
        << "] Buy "
        << ToPrettyStr(items[i].first) << " for "
        << items[i].second << " gold pieces\n"
      ;
      if (item.second <= character.GetGold()) { user_inputs.push_back(i + 1); }
      m_chapter->m_signal_show_text(s.str());
    }
    assert(!user_inputs.empty());
    if (user_inputs.size() == 1)
    {
      m_chapter->m_signal_show_text("You do not have enough money to buy anything.\n");
      break;
    }

    //Shop
    const int command{*m_chapter->m_signal_request_input(user_inputs)};
    if (command == 0) break;
    const int i = command - 1;
    if (i < 0 || i >= static_cast<int>(items.size()))
    {
      m_chapter->m_signal_show_text("Invalid number. Choose zero to leave the shop or an item number to buy it.\n");
      continue;
    }
    assert(i >= 0);
    assert(i < static_cast<int>(items.size()));
    if (character.GetGold() < items[i].second)
    {
      m_chapter->m_signal_show_text("Cannot buy this item: not enough gold\n");
      continue;
    }
    m_chapter->m_signal_show_text("You bough " + ToPrettyStr(items[i].first) + "\n");
    character.AddItem(items[i].first);
    character.ChangeGold(-items[i].second);
    std::swap(items[i],items.back());
    items.pop_back();
  }
}

ShopChapter ParseShopChapter(std::stringstream& s, Chapter * const chapter)
{
  ShopChapter c(chapter);
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
