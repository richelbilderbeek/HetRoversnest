#ifndef SPECIALCHAPTER_H
#define SPECIALCHAPTER_H

struct Character;

void DoGameOver();
void DoGameWon();

void DoPlayBall(Character& character, const bool auto_play); //Chapter 378
void DoPlayDice(Character& character, const bool auto_play); //Chapter 206
void DoPlayPill(Character& character, const bool auto_play); //Chapter 223

#endif // DOCHAPTER_H
