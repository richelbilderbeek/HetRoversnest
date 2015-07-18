#ifndef SHOPCHAPTER_H
#define SHOPCHAPTER_H

#include <vector>

#include "item.h"

struct Character;

///Chapter where you can buy stuff
struct ShopChapter
{
  ShopChapter();

  void AddItem(const Item item, const int price);

  void Do(Character& character, const bool auto_play) const;

  const std::vector<std::pair<Item,int>>& GetItems() const noexcept { return m_items; }


  private:

  std::vector<std::pair<Item,int>> m_items;
};

ShopChapter ParseShopChapter(std::stringstream& s);

#endif // SHOPCHAPTER_H
