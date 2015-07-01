#ifndef DOCHAPTER_H
#define DOCHAPTER_H

#include <string>
#include <vector>
#include "language.h"

struct Character;

void DoChangeStatusChapter(std::stringstream& s, int& chapter, Character& character);
void DoChapter(int& chapter, Character& character, const Language language, const bool auto_play = false);
void DoFight(std::stringstream& s, int& chapter, Character& character, const bool auto_play);
void DoFightWithTime(std::stringstream& s, int& chapter, Character& character, const bool auto_play);
void DoGameOver(int& chapter);
void DoHasItemChapter(std::stringstream& s, int& chapter, Character& character);
void DoNormalChapter(std::stringstream& s, int& chapter, const bool auto_play);
void DoTestYourDexterityChapter(std::stringstream& s, int& chapter, Character& character);
void DoTestYourLuckChapter(std::stringstream& s, int& chapter, Character& character);

#endif // DOCHAPTER_H
