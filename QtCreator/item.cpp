#include "item.h"

#include <cassert>
#include <string>

std::string ToStr(const Item item)
{
  switch (item)
  {
    case Item::shield: return "shield";
    case Item::chain_mail: return "chain mail";
    case Item::fire_ring: return "fire ring";
    case Item::black_pearls: return "black pearls";
    case Item::witch_hair: return "witch hair";
    case Item::lotus_flower: return "lotus flower";
    case Item::tattoo: return "tattoo";
    case Item::brooche: return "brooch";
    case Item::silver_arrow: return "silver arrow";
    case Item::mirror: return "mirror";
    case Item::ice_ring: return "ice ring";
    case Item::invisibility_ring: return "invisibility ring";
    case Item::insect_bracelet: return "insect bracelet";
    case Item::throwing_knife: return "throwing knife";
    case Item::climbing_rope: return "climbing rope";
    case Item::meat_hook: return "meat hook";
    case Item::iron_spike: return "iron spike";
    case Item::lantern: return "lantern";
    case Item::merchant_pass: return "merchant pass";
    case Item::flowers: return "flowers";
    case Item::skeleton_key: return "skeleton key";
    case Item::random: return "random";
  }
  assert(!"Unknown item");
}

std::ostream& operator<<(std::ostream& os, const Item item)
{
  os << ToStr(item);
  return os;
}
