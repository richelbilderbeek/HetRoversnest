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
  brooche = 8, //Unknown which
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
  flowers = 20,
  skeleton_key = 21,
  iron_key = 22,
  silver_brooch = 23, //Heals one stamina after each battle
  ring_of_the_golden_eye = 24,
  random = 99, //For chapter 63, where you lose two random items
  //Below this, these are status changes
  fought_ape_man = 100 //Chapter 201
};

std::string ToStr(const Item item);
std::ostream& operator<<(std::ostream& os, const Item item);

#endif // ITEM_H
