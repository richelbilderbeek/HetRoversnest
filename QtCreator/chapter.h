#ifndef CHAPTER_H
#define CHAPTER_H

#include <string>
#include <vector>

#include "chaptertype.h"
#include "character.h"
#include "monster.h"

struct Chapter
{
  Chapter(const std::string& filename);

  ///Which items are added to the player's inventory?
  std::set<Item> GetAddItems() const noexcept { return m_add_items; }

  ///The text displayed at the end of the chapter
  const std::string& GetByeText() const noexcept { return m_bye_text; }

  ///How will the gold of the character change in this chapter?
  int GetChangeGold() const noexcept { return m_change_gold; }

  ///How will the luck of the character change in this chapter?
  int GetChangeLuck() const noexcept { return m_change_luck; }

  void Do(Character& character, const bool auto_play) const;

  ///Battle the monsters one by one
  bool DoFightSequentially() const noexcept { return m_fight_sequentially; }

  int GetNextChapter() const noexcept { return m_next_chapter; }
  const std::vector<Monster>& GetMonsters() const noexcept { return m_monsters; }

  ///At which round can the player escape? 1000 if player cannot
  int GetRoundsToEscape() const noexcept { return m_rounds_to_escape; }

  ///If the player escapes, which chapter will he/she go to? -1 if player cannot escape
  int GetEscapeToChapter() const noexcept { return m_escape_chapter; }

  std::string GetText() const noexcept { return m_text; }

  ChapterType GetType() const noexcept { return m_chapter_type; }

  //bool IsGameOver() const noexcept { return m_is_game_over; }
  //bool IsGameWon() const noexcept { return m_is_game_won; }

  private:
  ///Which items are added to the player's inventory?
  std::set<Item> m_add_items;

  ///The text displayed at the end of the chapter
  std::string m_bye_text;

  ///How much will the gold of the character change?
  int m_change_gold;

  ///How much will the luck of the character change?
  int m_change_luck;

  ChapterType m_chapter_type;

  ///If the player escapes a monster fight, to which chapter?
  int m_escape_chapter;

  ///Is the fight sequentially (one after one) or are the monster fought both at the same time?
  bool m_fight_sequentially;

  std::vector<Monster> m_monsters;
  int m_next_chapter; //When there is no choice
  int m_rounds_to_escape;
  std::string m_text;
};

std::ostream& operator<<(std::ostream& os, const Chapter& chapter);

#endif // CHAPTER_H
