#ifndef PAWNSHOPCHAPTER_H
#define PAWNSHOPCHAPTER_H

#include <iosfwd>
#include <vector>

#include "showtextmode.h"
#include "item.h"

struct Character;

///Chapter where you can sell stuff
struct PawnShopChapter
{
  PawnShopChapter();

  void AddItem(const Item item, const int price);

  void Do(Character& character, const ShowTextMode text_mode) const;

  const std::vector<std::pair<Item,int>>& GetItems() const noexcept { return m_items; }

  private:

  std::vector<std::pair<Item,int>> m_items;
};

PawnShopChapter ParsePawnShopChapter(std::stringstream& s);

#endif // PAWNSHOPCHAPTER_H
