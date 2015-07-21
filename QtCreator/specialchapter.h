#ifndef SPECIALCHAPTER_H
#define SPECIALCHAPTER_H

#include "showtextmode.h"
struct Character;

void DoGameOver(const ShowTextMode text_mode);
void DoGameWon(const ShowTextMode text_mode);

void DoPlayBall(Character& character, const ShowTextMode text_mode); //Chapter 378
void DoPlayDice(Character& character, const ShowTextMode text_mode); //Chapter 206
void DoPlayPill(Character& character, const ShowTextMode text_mode); //Chapter 223

#endif // DOCHAPTER_H
