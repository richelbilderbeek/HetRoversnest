#ifndef ITEM_H
#define ITEM_H

#include <iosfwd>

enum class Item
{
  shield = 1,
  chain_mail = 2,
  fire_ring = 3,
  black_pearls = 4,
  witch_hair = 5,
  lotus_flower = 6,
  tattoo = 7,
  brooche = 8,
  silver_arrow = 9,
  mirror = 10,
  ice_ring = 11,
  invisibility_ring = 12
};

std::string ToStr(const Item item);
std::ostream& operator<<(std::ostream& os, const Item item);

#endif // ITEM_H
