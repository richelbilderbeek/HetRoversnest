#ifndef DOCHAPTER_H
#define DOCHAPTER_H

#include <string>
#include <vector>
#include "item.h"
#include "language.h"

struct Character;
struct Monster;

void DoChangeStatusChapter(std::stringstream& s, int& chapter, Character& character);
void DoChapter(int& chapter, Character& character, const Language language, const bool auto_play = false);

///Fight multiple monsters after each other
void DoFight(
  std::vector<Monster> monsters,
  Character& character,
  const bool auto_play
);

///Fight multiple monsters at the same time
void DoFightTwoMonsters(
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
void DoGameOver();
void DoGameWon();
void DoHasItemChapter(std::stringstream& s, int& chapter, Character& character);

void DoNormalChapter(
  std::vector<std::pair<char,int>> options,
  int& chapter,
  const bool auto_play
);

void DoShop(
  std::vector<std::pair<Item,int>> items,
  const std::string& exit_text,
  Character& character,
  const bool auto_play
);

void DoTestYourDexterityChapter(std::stringstream& s, int& chapter, Character& character);
void DoTestYourLuckChapter(std::stringstream& s, int& chapter, Character& character);

void Parse(std::stringstream& s, const char expected_char);

void ParseShop(
  std::stringstream& s,
  int& chapter,
  Character& character,
  const bool auto_play
);


void ParseChangeStatus(std::stringstream& s, Character& character);
void ParseChangeStatusAskOption(
  std::stringstream& s,
  int& chapter,
  Character& character,
  const bool auto_play
);

void ParseFight(std::stringstream& s, int& chapter, Character& character, const bool auto_play);
void ParseFightWithRandomMonster(std::stringstream& s, int& chapter, Character& character, const bool auto_play);
void ParseFightWithTwoMonsters(std::stringstream& s, int& chapter, Character& character, const bool auto_play);
void ParseNormalChapter(
  std::stringstream& s,
  int& chapter,
  const Character& character,
  const bool auto_play
);

char ReadChar(std::stringstream& s);
Item ReadItem(std::stringstream& s);
int ReadInt(std::stringstream& s);

#endif // DOCHAPTER_H
