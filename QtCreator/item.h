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
  golden_brooch = 8, //Gives 2 luck, chapter 8
  silver_arrow = 9,
  mirror = 10,
  ice_ring = 11,
  invisibility_ring = 12,
  insect_bracelet = 13,
  throwing_knife = 14,
  climbing_rope = 15,
  meat_hook = 16,
  iron_spike = 17,
  lantern = 18,
  merchant_pass = 19,
  flowers = 20, //The Gold Flower
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
  silver_bracelet = 38, //?Where to find it?
  helmet = 39,
  silver_flute = 40, //Obtained at 359
  eye_patch = 41,
  piece_of_chalk = 43,
  //Abstract items
  all_silver_items = 94, //For chapter 343
  something_silver = 95, //For chapter 328
  some_piece_of_armor = 96, //For chapter 293, where Miss Pipe request some piece of armor
  two_magic_items = 97, //For chapter 248, where Ben Borryman wants two magic items
  something_magic = 98, //For chapter 187 and 293, where the guards/Miss Pipe request something magic
  random = 99, //For chapter 63, where you lose two random items
  //Below this, these are status changes
  fought_ape_man = 100 //Chapter 201
};

std::string ToStr(const Item item);
std::ostream& operator<<(std::ostream& os, const Item item);

#endif // ITEM_H
