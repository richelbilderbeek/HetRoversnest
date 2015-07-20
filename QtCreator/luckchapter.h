#ifndef LUCKCHAPTER_H
#define LUCKCHAPTER_H

#include <string>
#include "consequence.h"

struct Character;

///Chapter in which you have to test your luck
struct LuckChapter
{
  LuckChapter();

  ///Let the character undergo this chapter
  void Do(Character& character, const bool auto_play) const;

  const std::string& GetLuckText() const noexcept { return m_luck_text; }
  const std::string& GetNoLuckText() const noexcept { return m_no_luck_text; }
  int GetLuckChapter() const noexcept { return m_luck_consequence.GetNextChapter(); }
  int GetNoLuckChapter() const noexcept { return m_no_luck_consequence.GetNextChapter(); }

  void SetLuckConsequence(const Consequence& luck_consequence) noexcept { m_luck_consequence = luck_consequence; }
  void SetLuckText(const std::string& luck_text) noexcept { m_luck_text = luck_text; }
  void SetNoLuckText(const std::string& no_luck_text) noexcept { m_no_luck_text = no_luck_text; }
  void SetLuckChapter(const int luck_chapter) noexcept;
  void SetNoLuckChapter(const int no_luck_chapter) noexcept;
  void SetNoLuckConsequence(const Consequence& no_luck_consequence) noexcept { m_no_luck_consequence = no_luck_consequence; }

  private:

  Consequence m_luck_consequence;
  std::string m_luck_text;
  Consequence m_no_luck_consequence;
  std::string m_no_luck_text;

};

#endif // LUCKCHAPTER_H
