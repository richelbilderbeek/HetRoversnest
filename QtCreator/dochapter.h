#ifndef DOCHAPTER_H
#define DOCHAPTER_H

//#include <string>
//#include <vector>
//#include "consequence.h"
//#include "item.h"
//#include "language.h"

struct Character;
//struct Monster;

void DoGameOver();
void DoGameWon();

void DoInventory(
  Character& character,
  const bool auto_play
);

void DoPlayBall(Character& character, const bool auto_play); //Chapter 378
void DoPlayDice(Character& character, const bool auto_play); //Chapter 206
void DoPlayPill(Character& character, const bool auto_play); //Chapter 223

#endif // DOCHAPTER_H
