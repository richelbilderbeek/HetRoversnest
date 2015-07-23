#include "pawnshopchapter.h"

#include <cassert>
#include <sstream>

#include "chapter.h"
#include "character.h"
#include "helper.h"

PawnShopChapter::PawnShopChapter(Chapter * const chapter)
  : m_chapter{chapter}, m_items{}
{

}

void PawnShopChapter::AddItem(const Item item, const int price)
{
  m_items.push_back(std::make_pair(item,price));
}

void PawnShopChapter::Do(Character& character) const
{
  //Must be a copy, as the copy can get cleared
  std::vector<std::pair<Item,int>> items = this->GetItems();

  while (1)
  {
    std::vector<int> user_options;
    std::stringstream s;
    s << "[0] Leave shop\n";
    user_options.push_back(0);
    const int n_items{static_cast<int>(items.size())};
    for (int i=0; i!=n_items; ++i)
    {
      s
        << '[' << (character.HasItem(items[i].first) ? std::to_string(i+1) : " ") << "] Sell "
        << ToPrettyStr(items[i].first) << " for "
        << items[i].second << " gold pieces\n"
      ;
      if (character.HasItem(items[i].first)) { user_options.push_back(i + 1); }
    }
    m_chapter->m_signal_show_text(s.str());

    if (user_options.size() == 1)
    {
      m_chapter->m_signal_show_text("You do not have any items to sell.\n");
      break;
    }

    //Pawn shop
    const int command{*m_chapter->m_signal_request_input(user_options)};
    if (command == 0) break;
    const int i = command - 1;
    if (i < 0 || i >= static_cast<int>(items.size()))
    {
      m_chapter->m_signal_show_text("Invalid number. Choose zero to leave the shop or an item number to buy it.\n");
      continue;
    }
    assert(i >= 0);
    assert(i < static_cast<int>(items.size()));
    if (!character.HasItem(items[i].first))
    {
      m_chapter->m_signal_show_text("You do not possess this item.\n");
      continue;
    }
    m_chapter->m_signal_show_text("You sold " + ToPrettyStr(items[i].first) + "\n");
    character.RemoveItem(items[i].first);
    character.ChangeGold(items[i].second);
    std::swap(items[i],items.back());
    items.pop_back();
  }
}

PawnShopChapter ParsePawnShopChapter(std::stringstream& s, Chapter * const chapter)
{
  PawnShopChapter c(chapter);
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
