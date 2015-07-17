#ifndef ITEM_H
#define ITEM_H

#include <iosfwd>

enum class Item
{
  shield = 1,
  chainmail_coat = 2,
  ring_of_fire = 3,
  black_pearls = 4,
  hags_hair = 5,
  lotus_flower = 6,
  tattoo = 7,
  golden_brooch = 8, //Gives 2 luck, chapter 8
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
  gold_flower = 20, //The Gold Flower
  skeleton_key = 21,
  iron_key = 22,
  silver_brooch = 23, //Heals one stamina after each battle
  ring_of_the_golden_eye = 24,
  glass_ball = 26,
  ivory_skull = 27,
  silver_chain = 28,
  garlic = 29,
  knucklebones = 30, //NL: bikkels
  golden_owl = 31,
  potion_of_mind_control = 32, //Obtained at 359
  set_of_keys = 33,
  stale_bread = 34,
  two_gems = 35,
  magnificent_shield = 36, //Found in chapter 340, raises attack strength by 1
  silver_goblet = 37, //?Where to find it?
  magic_helmet = 39, //Found in chapter 376. add 1 point to attack
  silver_flute = 40, //Obtained at 359
  eye_patch = 41,
  elven_boots = 42,
  piece_of_chalk = 43,
  shield_with_uniform_crest = 44, //Found in chapter 374
  copper_brooch = 45, //Lose 1 luck without knowing
  carralifs_sword = 46, //Present at start, can lose it at 126
  ordinary_sword = 47, //Obtain it at 126
  luck_potion = 48,
  stamina_potion = 49,
  dexterity_potion = 50,
  //Abstract items
  all_provisions = 91, //For chapter 42
  random_item_or_one_gold = 92, //For chapter 148
  any_item = 93, //For chapter 113, where you give a selected item
  all_silver_items = 94, //For chapter 343
  something_silver = 95, //For chapter 328
  some_piece_of_armor = 96, //For chapter 293, where Miss Pipe request some piece of armor
  two_magic_items = 97, //For chapter 248, where Ben Borryman wants two magic items
  something_magic = 98, //For chapter 187 and 293, where the guards/Miss Pipe request something magic
  random_item = 99, //For chapter 63, where you lose two random items
  //Below this, these are status changes
  fought_ape_man = 100 //Chapter 201
};

std::string ToStr(const Item item);
Item ToItem(const std::string& item_name);
bool IsItem(const std::string& item_name);
std::ostream& operator<<(std::ostream& os, const Item item);

#endif // ITEM_H
