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
    case Item::iron_key: return "iron key";
    case Item::silver_brooch: return "silver brooch";
    case Item::ring_of_the_golden_eye: return "ring of the golden eye";
    case Item::golden_brooch: return "golden brooch";
    case Item::glass_ball: return "glass ball";
    case Item::ivory_skull: return "ivory skull";
    case Item::silver_chain: return "silver chain";
    case Item::garlic: return "garlic";
    case Item::knucklebones: return "knucklebones";
    case Item::golden_owl: return "golden owl";
    case Item::potion_of_mind_control: return "potion of mind control";
    case Item::set_of_keys: return "set of keys";
    case Item::stale_bread: return "stale bread";
    case Item::two_gems: return "two gems";
    case Item::magnificent_shield: return "magnificent shield";
    case Item::silver_goblet: return "silver goblet";
    case Item::silver_bracelet: return "silver bracelet";
    case Item::magic_helmet: return "magic_helmet";
    case Item::silver_flute: return "silver flute";
    case Item::eye_patch: return "eye patch";
    case Item::elven_boots: return "elven boots";
    case Item::piece_of_chalk: return "piece_of_chalk";
    case Item::shield_with_uniform_crest: return "shield with uniform crest";
    case Item::copper_brooch: return "copper brooch";
    case Item::carralifs_sword: return "Carralif's sword";
    case Item::ordinary_sword: return "ordinary sword";
    case Item::luck_potion: return "luck_potion";
    case Item::stamina_potion: return "stamina_potion";
    case Item::dexterity_potion: return "dexterity_potion";
    case Item::random_item_or_one_gold: return "random_item_or_one_gold";
    case Item::any_item: return "any_item";
    case Item::all_silver_items: return "all_silver_items";
    case Item::something_silver: return "something silver";
    case Item::some_piece_of_armor: return "some piece of armor";
    case Item::random_item: return "random_item";
    case Item::two_magic_items: return "two magic items";
    case Item::something_magic: return "something magic";
    case Item::fought_ape_man: return "fought_ape_man";
  }
  assert(!"Unknown item");
}

std::ostream& operator<<(std::ostream& os, const Item item)
{
  os << ToStr(item);
  return os;
}
