#include "item.h"

#include <cassert>
#include <string>

std::string ToStr(const Item item)
{
  switch (item)
  {
    case Item::shield: return "shield";
    case Item::chain_mail: return "chain_mail";
    case Item::fire_ring: return "fire_ring";
    case Item::black_pearls: return "black_pearls";
    case Item::witch_hair: return "witch_hair";
    case Item::lotus_flower: return "lotus_flower";
    case Item::tattoo: return "tattoo";
  }
  assert(!"Unknown item");
}

std::ostream& operator<<(std::ostream& os, const Item item)
{
  os << ToStr(item);
  return os;
}
