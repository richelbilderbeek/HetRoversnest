#ifndef CHAPTER_H
#define CHAPTER_H

#include <string>
#include <vector>

#include "chaptertype.h"
#include "character.h"
#include "monster.h"
#include "fightingchapter.h"
#include "optionschapter.h"

struct Chapter
{
  Chapter(const std::string& filename);

  ///Let the player do this chapter
  void Do(Character& character, const bool auto_play) const;

  ///Which items are added to the player's inventory?
  std::set<Item> GetAddItems() const noexcept { return m_add_items; }

  ///The text displayed at the end of the chapter
  const std::string& GetByeText() const noexcept { return m_bye_text; }

  ///How will the dexterity of the character change in this chapter?
  int GetChangeDexterity() const noexcept { return m_change_dex; }

  ///How will the gold of the character change in this chapter?
  int GetChangeGold() const noexcept { return m_change_gold; }

  ///How will the luck of the character change in this chapter?
  int GetChangeLuck() const noexcept { return m_change_luck; }

  ///How will the stamina of the character change in this chapter?
  int GetChangeStamina() const noexcept { return m_change_sta; }

  int GetNextChapter() const noexcept { return m_next_chapter; }

  const FightingChapter& GetFighting() const noexcept { return m_fighting_chapter; }
  FightingChapter& GetFighting() noexcept { return m_fighting_chapter; }

  const OptionsChapter& GetOptions() const noexcept { return m_options_chapter; }
  OptionsChapter& GetOptions() noexcept { return m_options_chapter; }

  std::string GetText() const noexcept { return m_text; }

  ChapterType GetType() const noexcept { return m_chapter_type; }

  private:
  ///Which items are added to the player's inventory?
  std::set<Item> m_add_items;

  ///The text displayed at the end of the chapter
  std::string m_bye_text;

  ///How much will the dexterity of the character change?
  int m_change_dex;

  ///How much will the gold of the character change?
  int m_change_gold;

  ///How much will the luck of the character change?
  int m_change_luck;

  ///How much will the stamina of the character change?
  int m_change_sta;

  ChapterType m_chapter_type;

  FightingChapter m_fighting_chapter;


  int m_next_chapter; //When there is no choice

  OptionsChapter m_options_chapter;

  ///Which items are removed to the player's inventory?
  std::set<Item> m_remove_items;

  std::string m_text;
};

std::ostream& operator<<(std::ostream& os, const Chapter& chapter);

#endif // CHAPTER_H
