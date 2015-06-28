#ifndef DOCHAPTER_H
#define DOCHAPTER_H

#include <string>
#include <vector>

struct Character;

void DoChapter(int& chapter, Character& character, const bool auto_play = false);
void DoNormalChapter(std::stringstream& s, int& chapter, const bool auto_play);
void DoTestYourLuckChapter(std::stringstream& s, int& chapter, Character& character);

#endif // DOCHAPTER_H
