#include "item.h"

#include <boost/bimap.hpp>

#include "helper.h"

boost::bimap<Item,std::string> CreateItemBimap()
{
  static boost::bimap<Item,std::string> m;
  {
    static bool is_done{false};
    if (is_done) return m;
    is_done = true;
  }
  typedef boost::bimap<Item,std::string>::value_type Pair;
  m.insert(Pair(Item::all_gold,"all_gold"));
  m.insert(Pair(Item::all_needed_to_slay_zanbar_bone,"all_needed_to_slay_zanbar_bone"));
  m.insert(Pair(Item::all_provisions,"all_provisions"));
  m.insert(Pair(Item::all_silver_items,"all_silver_items"));
  m.insert(Pair(Item::any_scorpion_brooch,"any_scorpion_brooch"));
  m.insert(Pair(Item::black_pearls,"black_pearls"));
  m.insert(Pair(Item::carralifs_sword,"carralifs_sword"));
  m.insert(Pair(Item::chainmail_coat,"chainmail_coat"));
  m.insert(Pair(Item::climbing_rope,"climbing_rope"));
  m.insert(Pair(Item::coloured_candle,"coloured_candle"));
  m.insert(Pair(Item::copper_scorpion_brooch,"copper_scorpion_brooch"));
  m.insert(Pair(Item::cursed_white_silk_glove,"cursed_white_silk_glove"));
  m.insert(Pair(Item::dexterity_potion,"dexterity_potion"));
  m.insert(Pair(Item::eye_patch,"eye_patch"));
  m.insert(Pair(Item::followed_sewer_north,"followed_sewer_north"));
  m.insert(Pair(Item::followed_sewer_south,"followed_sewer_south"));
  m.insert(Pair(Item::garlic,"garlic"));
  m.insert(Pair(Item::glass_ball,"glass_ball"));
  m.insert(Pair(Item::golden_owl,"golden_owl"));
  m.insert(Pair(Item::golden_scorpion_brooch,"golden_scorpion_brooch"));
  m.insert(Pair(Item::gold_flower,"gold_flower"));
  m.insert(Pair(Item::hags_hair,"hags_hair"));
  m.insert(Pair(Item::has_inspected_suit_of_armor,"has_inspected_suit_of_armor"));
  m.insert(Pair(Item::invisibility_ring,"invisibility_ring"));
  m.insert(Pair(Item::iron_key,"iron_key"));
  m.insert(Pair(Item::iron_spike,"iron_spike"));
  m.insert(Pair(Item::ivory_skull_on_a_silver_chain,"ivory_skull_on_a_silver_chain"));
  m.insert(Pair(Item::seen_mummy,"seen_mummy"));
  m.insert(Pair(Item::knucklebones,"knucklebones"));
  m.insert(Pair(Item::lantern,"lantern"));
  m.insert(Pair(Item::lifted_goblet_a,"lifted_goblet_a"));
  m.insert(Pair(Item::lifted_goblet_b,"lifted_goblet_b"));
  m.insert(Pair(Item::lifted_goblet_c,"lifted_goblet_c"));
  m.insert(Pair(Item::lotus_flower,"lotus_flower"));
  m.insert(Pair(Item::luck_potion,"luck_potion"));
  m.insert(Pair(Item::magic_elven_boots,"magic_elven_boots"));
  m.insert(Pair(Item::magic_helmet,"magic_helmet"));
  m.insert(Pair(Item::magnificent_shield,"magnificent_shield"));
  m.insert(Pair(Item::meat_hook,"meat_hook"));
  m.insert(Pair(Item::merchant_pass,"merchant_pass"));
  m.insert(Pair(Item::mirror,"mirror"));
  m.insert(Pair(Item::opened_pirate_captain_door,"opened_pirate_captain_door"));
  m.insert(Pair(Item::opened_pirate_crew_door,"opened_pirate_crew_door"));
  m.insert(Pair(Item::ordinary_sword,"ordinary_sword"));
  m.insert(Pair(Item::picked_up_golden_scorpion,"picked_up_golden_scorpion"));
  m.insert(Pair(Item::picked_up_silver_scorpion,"picked_up_silver_scorpion"));
  m.insert(Pair(Item::piece_of_chalk,"piece_of_chalk"));
  m.insert(Pair(Item::potion_of_mind_control,"potion_of_mind_control"));
  m.insert(Pair(Item::random_item_or_one_gold,"random_item_or_one_gold"));
  m.insert(Pair(Item::ring_of_fire,"ring_of_fire"));
  m.insert(Pair(Item::ring_of_ice,"ring_of_ice"));
  m.insert(Pair(Item::ring_of_the_golden_eye,"ring_of_the_golden_eye"));
  m.insert(Pair(Item::set_of_keys,"set_of_keys"));
  m.insert(Pair(Item::shield,"shield"));
  m.insert(Pair(Item::shield_with_tower_crest,"shield_with_tower_crest"));
  m.insert(Pair(Item::shield_with_unicorn_crest,"shield_with_unicorn_crest"));
  m.insert(Pair(Item::silver_arrow,"silver_arrow"));
  m.insert(Pair(Item::silver_chalice,"silver_chalice"));
  m.insert(Pair(Item::silver_flute,"silver_flute"));
  m.insert(Pair(Item::silver_insect_bracelet,"silver_insect_bracelet"));
  m.insert(Pair(Item::silver_scorpion_brooch,"silver_scorpion_brooch"));
  m.insert(Pair(Item::silver_spoon,"silver_spoon"));
  m.insert(Pair(Item::skeleton_key,"skeleton_key"));
  m.insert(Pair(Item::stale_brown_bread,"stale_brown_bread"));
  m.insert(Pair(Item::stale_white_bread,"stale_white_bread"));
  m.insert(Pair(Item::stamina_potion,"stamina_potion"));
  m.insert(Pair(Item::tattoo,"tattoo"));
  m.insert(Pair(Item::throwing_knife,"throwing_knife"));
  m.insert(Pair(Item::two_gems,"two_gems"));
  m.insert(Pair(Item::two_random_items,"two_random_items"));
  m.insert(Pair(Item::two_silver_goblets,"two_silver_goblets"));
  return m;
}

bool IsItem(const std::string& item_name)
{
  const auto m = CreateItemBimap();
  return m.right.find(item_name) != m.right.end();
}

bool IsMagic(const Item item)
{
  return
       item == Item::magic_elven_boots
    || item == Item::magic_helmet
    || item == Item::silver_scorpion_brooch
    || item == Item::golden_scorpion_brooch
    || item == Item::copper_scorpion_brooch
    || item == Item::invisibility_ring
    || item == Item::ring_of_fire
    || item == Item::ring_of_ice
    || item == Item::ring_of_the_golden_eye
    || item == Item::silver_insect_bracelet
  ;
}

bool IsSilver(const Item item)
{
  return
       item == Item::ivory_skull_on_a_silver_chain
    || item == Item::silver_arrow
    || item == Item::silver_chalice
    || item == Item::silver_flute
    || item == Item::silver_insect_bracelet
    || item == Item::silver_scorpion_brooch
    || item == Item::silver_spoon
    || item == Item::two_silver_goblets
  ;
}

Item ReadItem(std::stringstream& s)
{
  int number = -1;
  s >> number;
  assert(number > -1);
  const Item item = static_cast<Item>(number);
  return item;
}

Item ToItem(const std::string& item_name)
{
  const auto m = CreateItemBimap();
  assert(IsItem(item_name));
  return m.right.find(item_name)->second;
}

std::string ToPrettyStr(const Item item)
{
  return ToPretty(ToStr(item));
}

std::string ToStr(const Item item)
{
  const auto m = CreateItemBimap();
  assert(m.left.find(item) != m.left.end());
  return m.left.find(item)->second;
}

std::ostream& operator<<(std::ostream& os, const Item item)
{
  os << ToStr(item);
  return os;
}
