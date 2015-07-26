#ifndef ITEM_H
#define ITEM_H

#include <iosfwd>
#include <string>

enum class Item
{
  shield = 1,
  chainmail_coat = 2,
  ring_of_fire = 3,
  black_pearls = 4,
  hags_hair = 5,
  lotus_flower = 6,
  tattoo = 7,
  golden_scorpion_brooch = 8, //Gives 2 luck, chapter 8
  silver_arrow = 9,
  mirror = 10,
  ring_of_ice = 11,
  invisibility_ring = 12,
  silver_insect_bracelet = 13,
  throwing_knife = 14,
  climbing_rope = 15,
  meat_hook = 16,
  iron_spike = 17,
  lantern = 18,
  merchant_pass = 19,
  gold_flower = 20,
  skeleton_key = 21,
  iron_key = 22,
  silver_scorpion_brooch = 23, //Heals one condition after each battle
  ring_of_the_golden_eye = 24,
  glass_ball = 26,
  ivory_skull_on_a_silver_chain = 27,
  garlic = 29,
  knucklebones = 30, //NL: bikkels
  golden_owl = 31,
  potion_of_mind_control = 32, //Obtained at 359
  set_of_keys = 33, //Obtained at 285
  stale_white_bread = 34,
  two_gems = 35,
  magnificent_shield = 36, //Found in chapter 340, raises attack strength by 1
  two_silver_goblets = 37, //Chapter 220
  magic_helmet = 39, //Found in chapter 376. add 1 point to attack
  silver_flute = 40, //Obtained at 359
  eye_patch = 41,
  magic_elven_boots = 42,
  piece_of_chalk = 43,
  shield_with_unicorn_crest = 44, //Found in chapter 374
  copper_scorpion_brooch = 45, //Lose 1 luck without knowing
  carralifs_sword = 46, //Present at start, can lose it at 126
  ordinary_sword = 47, //Obtain it at 126
  luck_potion = 48,
  stamina_potion = 49,
  dexterity_potion = 50,
  silver_chalice = 51, //Chapter 220
  silver_spoon = 52, //Chapter 220
  coloured_candle = 53, //Chapter 241
  shield_with_tower_crest = 54, //Found in chapter 125
  cursed_white_silk_glove = 55,
  stale_brown_bread = 56,
  //Abstract items
  any_scorpion_brooch = 89,
  all_gold = 90, //For chapter 42
  all_provisions = 91, //For chapter 42
  random_item_or_one_gold = 92, //For chapter 148
  all_silver_items = 94, //For chapter 343
  two_magic_items = 97, //For chapter 248, where Ben Borryman wants two magic items
  something_magic = 98, //For chapter 187 and 293, where the guards/Miss Pipe request something magic
  two_random_items = 99, //For chapter 63, where you lose two random items
  //Below this, these are status changes
  followed_sewer_north = 101, //Chapter 174
  followed_sewer_south = 102, //Chapter 118
  opened_pirate_crew_door, //Chapter 176
  opened_pirate_captain_door, //Chapter
  lifted_goblet_a,
  lifted_goblet_b, //Chapter 209
  lifted_goblet_c,
  picked_up_silver_scorpion,
  picked_up_golden_scorpion,
  has_inspected_suit_of_armor, //5th floor of Zanbar Bone's tower
  all_needed_to_slay_zanbar_bone
};

std::string ToPrettyStr(const Item item);
std::string ToStr(const Item item);

Item ToItem(const std::string& item_name);
bool IsItem(const std::string& item_name);
bool IsSilver(const Item item);

//Is magic and non-cursed
bool IsMagic(const Item item);
std::ostream& operator<<(std::ostream& os, const Item item);

Item ReadItem(std::stringstream& s);

#endif // ITEM_H
