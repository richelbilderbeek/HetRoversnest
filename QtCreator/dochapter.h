#ifndef DOCHAPTER_H
#define DOCHAPTER_H

#include <string>
#include <vector>
#include "language.h"

struct Character;
struct Monster;

void DoChangeStatusChapter(std::stringstream& s, int& chapter, Character& character);
void DoChapter(int& chapter, Character& character, const Language language, const bool auto_play = false);

///Fight multiple monsters
void DoFight(
  std::vector<Monster> monsters,
  Character& character,
  const bool auto_play
);

///Fight single monster
void DoFight(
  Monster monster,
  Character& character,
  const bool auto_play
);

void DoFightWithTime(std::stringstream& s, int& chapter, Character& character, const bool auto_play);
void DoGameOver(int& chapter);
void DoHasItemChapter(std::stringstream& s, int& chapter, Character& character);

void DoNormalChapter(
  std::vector<std::pair<char,int>> options,
  int& chapter,
  const bool auto_play
);

void DoTestYourDexterityChapter(std::stringstream& s, int& chapter, Character& character);
void DoTestYourLuckChapter(std::stringstream& s, int& chapter, Character& character);

void Parse(std::stringstream& s, const char expected_char);

void ParseChangeStatusAskOption(
  std::stringstream& s,
  int& chapter,
  Character& character,
  const bool auto_play
);

void ParseFight(std::stringstream& s, int& chapter, Character& character, const bool auto_play);
void ParseNormalChapter(
  std::stringstream& s,
  int& chapter,
  const Character& character,
  const bool auto_play
);

#endif // DOCHAPTER_H
