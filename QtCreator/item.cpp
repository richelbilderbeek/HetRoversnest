#include "item.h"

#include <cassert>
#include <string>
#include <boost/bimap.hpp>

boost::bimap<Item,std::string> CreateBimap()
{
  static boost::bimap<Item,std::string> m;
  {
    static bool is_done{false};
    if (is_done) return m;
    is_done = true;
  }
  typedef boost::bimap<Item,std::string>::value_type Pair;
  m.insert(Pair(Item::shield,"shield"));
  m.insert(Pair(Item::chainmail_coat,"chainmail_coat"));
  m.insert(Pair(Item::fire_ring,"fire_ring"));
  m.insert(Pair(Item::black_pearls,"black_pearls"));
  m.insert(Pair(Item::witch_hair,"witch_hair"));
  m.insert(Pair(Item::lotus_flower,"lotus_flower"));
  m.insert(Pair(Item::tattoo,"tattoo"));
  m.insert(Pair(Item::silver_arrow,"silver_arrow"));
  m.insert(Pair(Item::mirror,"mirror"));
  m.insert(Pair(Item::ice_ring,"ice_ring"));
  m.insert(Pair(Item::invisibility_ring,"invisibility_ring"));
  m.insert(Pair(Item::insect_bracelet,"insect_bracelet"));
  m.insert(Pair(Item::throwing_knife,"throwing_knife"));
  m.insert(Pair(Item::climbing_rope,"climbing_rope"));
  m.insert(Pair(Item::meat_hook,"meat_hook"));
  m.insert(Pair(Item::iron_spike,"iron_spike"));
  m.insert(Pair(Item::lantern,"lantern"));
  m.insert(Pair(Item::merchant_pass,"merchant_pass"));
  m.insert(Pair(Item::flowers,"flowers"));
  m.insert(Pair(Item::skeleton_key,"skeleton_key"));
  m.insert(Pair(Item::iron_key,"iron_key"));
  m.insert(Pair(Item::silver_brooch,"silver_brooch"));
  m.insert(Pair(Item::ring_of_the_golden_eye,"ring_of_the_golden_eye"));
  m.insert(Pair(Item::golden_brooch,"golden_brooch"));
  m.insert(Pair(Item::glass_ball,"glass_ball"));
  m.insert(Pair(Item::ivory_skull,"ivory_skull"));
  m.insert(Pair(Item::silver_chain,"silver_chain"));
  m.insert(Pair(Item::garlic,"garlic"));
  m.insert(Pair(Item::knucklebones,"knucklebones"));
  m.insert(Pair(Item::golden_owl,"golden_owl"));
  m.insert(Pair(Item::potion_of_mind_control,"potion_of_mind_control"));
  m.insert(Pair(Item::set_of_keys,"set_of_keys"));
  m.insert(Pair(Item::stale_bread,"stale_bread"));
  m.insert(Pair(Item::two_gems,"two_gems"));
  m.insert(Pair(Item::magnificent_shield,"magnificent_shield"));
  m.insert(Pair(Item::silver_goblet,"silver_goblet"));
  m.insert(Pair(Item::silver_bracelet,"silver_bracelet"));
  m.insert(Pair(Item::magic_helmet,"magic_helmet"));
  m.insert(Pair(Item::silver_flute,"silver_flute"));
  m.insert(Pair(Item::eye_patch,"eye_patch"));
  m.insert(Pair(Item::elven_boots,"elven_boots"));
  m.insert(Pair(Item::piece_of_chalk,"piece_of_chalk"));
  m.insert(Pair(Item::shield_with_uniform_crest,"shield_with_uniform_crest"));
  m.insert(Pair(Item::copper_brooch,"copper_brooch"));
  m.insert(Pair(Item::carralifs_sword,"Carralif's_sword"));
  m.insert(Pair(Item::ordinary_sword,"ordinary_sword"));
  m.insert(Pair(Item::luck_potion,"luck_potion"));
  m.insert(Pair(Item::stamina_potion,"stamina_potion"));
  m.insert(Pair(Item::dexterity_potion,"dexterity_potion"));
  m.insert(Pair(Item::random_item_or_one_gold,"random_item_or_one_gold"));
  m.insert(Pair(Item::any_item,"any_item"));
  m.insert(Pair(Item::all_silver_items,"all_silver_items"));
  m.insert(Pair(Item::something_silver,"something silver"));
  m.insert(Pair(Item::some_piece_of_armor,"some piece of armor"));
  m.insert(Pair(Item::random_item,"random_item"));
  m.insert(Pair(Item::two_magic_items,"two magic items"));
  m.insert(Pair(Item::something_magic,"something magic"));
  m.insert(Pair(Item::fought_ape_man,"fought_ape_man"));
  return m;
}

/*
static const boost::bimap<Item,std::string> m = CreateBimap();

  assert(m.left.find( 0) == m.left.end());
  assert(m.left.find( 4) != m.left.end());
  assert(m.left.find( 5) != m.left.end());
  assert(m.left.find(16) == m.left.end());
  assert(m.right.find( 0) == m.right.end());
  assert(m.right.find( 4) != m.right.end());
  assert(m.right.find( 5) == m.right.end());
  assert(m.right.find(16) != m.right.end());
*/

Item ToItem(const std::string& item_name)
{
  const auto m = CreateBimap();
  assert(m.right.find(item_name) != m.right.end());
  //return boost::get<0>(m.right.find(item_name));
  return m.right.find(item_name)->second;
}

std::string ToStr(const Item item)
{
  const auto m = CreateBimap();
  assert(m.left.find(item) != m.left.end());
  return m.left.find(item)->second;
  /*
  switch (item)
  {
    case Item::shield: return "shield";
    case Item::chainmail_coat: return "chainmail_coat";
    case Item::fire_ring: return "fire_ring";
    case Item::black_pearls: return "black_pearls";
    case Item::witch_hair: return "witch_hair";
    case Item::lotus_flower: return "lotus_flower";
    case Item::tattoo: return "tattoo";
    case Item::silver_arrow: return "silver_arrow";
    case Item::mirror: return "mirror";
    case Item::ice_ring: return "ice_ring";
    case Item::invisibility_ring: return "invisibility_ring";
    case Item::insect_bracelet: return "insect_bracelet";
    case Item::throwing_knife: return "throwing_knife";
    case Item::climbing_rope: return "climbing_rope";
    case Item::meat_hook: return "meat_hook";
    case Item::iron_spike: return "iron_spike";
    case Item::lantern: return "lantern";
    case Item::merchant_pass: return "merchant_pass";
    case Item::flowers: return "flowers";
    case Item::skeleton_key: return "skeleton_key";
    case Item::iron_key: return "iron_key";
    case Item::silver_brooch: return "silver_brooch";
    case Item::ring_of_the_golden_eye: return "ring_of_the_golden_eye";
    case Item::golden_brooch: return "golden_brooch";
    case Item::glass_ball: return "glass_ball";
    case Item::ivory_skull: return "ivory_skull";
    case Item::silver_chain: return "silver_chain";
    case Item::garlic: return "garlic";
    case Item::knucklebones: return "knucklebones";
    case Item::golden_owl: return "golden_owl";
    case Item::potion_of_mind_control: return "potion_of_mind_control";
    case Item::set_of_keys: return "set_of_keys";
    case Item::stale_bread: return "stale_bread";
    case Item::two_gems: return "two_gems";
    case Item::magnificent_shield: return "magnificent_shield";
    case Item::silver_goblet: return "silver_goblet";
    case Item::silver_bracelet: return "silver_bracelet";
    case Item::magic_helmet: return "magic_helmet";
    case Item::silver_flute: return "silver_flute";
    case Item::eye_patch: return "eye_patch";
    case Item::elven_boots: return "elven_boots";
    case Item::piece_of_chalk: return "piece_of_chalk";
    case Item::shield_with_uniform_crest: return "shield_with_uniform_crest";
    case Item::copper_brooch: return "copper_brooch";
    case Item::carralifs_sword: return "Carralif's_sword";
    case Item::ordinary_sword: return "ordinary_sword";
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
  */
}

std::ostream& operator<<(std::ostream& os, const Item item)
{
  os << ToStr(item);
  return os;
}
