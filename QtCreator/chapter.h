#ifndef CHAPTER_H
#define CHAPTER_H

#include <string>
#include <vector>

#include <boost/signals2.hpp>

#include "chaptertype.h"
#include "character.h"
#include "fightingchapter.h"
#include "luckchapter.h"
#include "optionschapter.h"
#include "pawnshopchapter.h"
#include "shopchapter.h"
#include "skillchapter.h"
#include "consequence.h"
#include "ballgamechapter.h"
#include "dicegamechapter.h"
#include "pillgamechapter.h"

struct Chapter
{
  Chapter(const int chapter_number);

  ///Let the player do this chapter
  void Do(Character& character) const;

  ///The text displayed at the end of the chapter
  const std::string& GetByeText() const noexcept { return m_bye_text; }

  int GetChapterNumber() const noexcept { return m_chapter_number; }

  const Consequence& GetConsequence() const noexcept { return m_consequence; }

  const FightingChapter& GetFighting() const noexcept { return m_fighting_chapter; }
  FightingChapter& GetFighting() noexcept { return m_fighting_chapter; }

  const LuckChapter& GetLuck() const noexcept { return m_luck_chapter; }
  LuckChapter& GetLuck() noexcept { return m_luck_chapter; }

  int GetNextChapter() const noexcept { return m_consequence.GetNextChapter(); }

  const OptionsChapter& GetOptions() const noexcept { return m_options_chapter; }
  OptionsChapter& GetOptions() noexcept { return m_options_chapter; }

  const PawnShopChapter& GetPawnShop() const noexcept { return m_pawn_shop_chapter; }
  PawnShopChapter& GetPawnShop() noexcept { return m_pawn_shop_chapter; }

  const ShopChapter& GetShop() const noexcept { return m_shop_chapter; }
  ShopChapter& GetShop() noexcept { return m_shop_chapter; }

  const SkillChapter& GetSkill() const noexcept { return m_skill_chapter; }
  SkillChapter& GetSkill() noexcept { return m_skill_chapter; }

  std::string GetText() const noexcept { return m_text; }

  ChapterType GetType() const noexcept { return m_chapter_type; }

  //If the Chapter wants an input
  mutable boost::signals2::signal<int(const std::vector<int>& valid_inputs)> m_signal_request_input;

  //If the Chapter want the dialog to display something
  mutable boost::signals2::signal<void(const std::string& text)> m_signal_show_text;

  //If the Chapter wants the dialog to wait
  mutable boost::signals2::signal<void()> m_signal_wait;

  private:

  BallGameChapter m_ball_game_chapter;

  ///The text displayed at the end of the chapter
  std::string m_bye_text;

  ///When there is no choice
  Consequence m_consequence;

  const int m_chapter_number;

  ChapterType m_chapter_type;

  DiceGameChapter m_dice_game_chapter;

  FightingChapter m_fighting_chapter;

  LuckChapter m_luck_chapter;

  OptionsChapter m_options_chapter;

  PawnShopChapter m_pawn_shop_chapter;

  PillGameChapter m_pill_game_chapter;

  ShopChapter m_shop_chapter;

  SkillChapter m_skill_chapter;

  std::string m_text;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

std::ostream& operator<<(std::ostream& os, const Chapter& chapter);

#endif // CHAPTER_H
