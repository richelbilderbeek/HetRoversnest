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
  m.insert(Pair(Item::ring_of_fire,"ring_of_fire"));
  m.insert(Pair(Item::black_pearls,"black_pearls"));
  m.insert(Pair(Item::hags_hair,"hags_hair"));
  m.insert(Pair(Item::lotus_flower,"lotus_flower"));
  m.insert(Pair(Item::tattoo,"tattoo"));
  m.insert(Pair(Item::silver_arrow,"silver_arrow"));
  m.insert(Pair(Item::mirror,"mirror"));
  m.insert(Pair(Item::ring_of_ice,"ring_of_ice"));
  m.insert(Pair(Item::invisibility_ring,"invisibility_ring"));
  m.insert(Pair(Item::silver_insect_bracelet,"silver_insect_bracelet"));
  m.insert(Pair(Item::throwing_knife,"throwing_knife"));
  m.insert(Pair(Item::climbing_rope,"climbing_rope"));
  m.insert(Pair(Item::meat_hook,"meat_hook"));
  m.insert(Pair(Item::iron_spike,"iron_spike"));
  m.insert(Pair(Item::lantern,"lantern"));
  m.insert(Pair(Item::merchant_pass,"merchant_pass"));
  m.insert(Pair(Item::gold_flower,"gold_flower"));
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
  m.insert(Pair(Item::magic_helmet,"magic_helmet"));
  m.insert(Pair(Item::silver_flute,"silver_flute"));
  m.insert(Pair(Item::eye_patch,"eye_patch"));
  m.insert(Pair(Item::elven_boots,"elven_boots"));
  m.insert(Pair(Item::piece_of_chalk,"piece_of_chalk"));
  m.insert(Pair(Item::shield_with_uniform_crest,"shield_with_uniform_crest"));
  m.insert(Pair(Item::copper_brooch,"copper_brooch"));
  m.insert(Pair(Item::carralifs_sword,"carralifs_sword"));
  m.insert(Pair(Item::ordinary_sword,"ordinary_sword"));
  m.insert(Pair(Item::luck_potion,"luck_potion"));
  m.insert(Pair(Item::stamina_potion,"stamina_potion"));
  m.insert(Pair(Item::dexterity_potion,"dexterity_potion"));
  m.insert(Pair(Item::all_provisions,"all_provisions"));
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

bool IsItem(const std::string& item_name)
{
  const auto m = CreateBimap();
  return m.right.find(item_name) != m.right.end();
}

Item ToItem(const std::string& item_name)
{
  const auto m = CreateBimap();
  assert(IsItem(item_name));
  return m.right.find(item_name)->second;
}

std::string ToStr(const Item item)
{
  const auto m = CreateBimap();
  assert(m.left.find(item) != m.left.end());
  return m.left.find(item)->second;
}

std::ostream& operator<<(std::ostream& os, const Item item)
{
  os << ToStr(item);
  return os;
}
