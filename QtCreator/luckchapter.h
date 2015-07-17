#ifndef LUCKCHAPTER_H
#define LUCKCHAPTER_H

#include <string>

struct Character;

///Chapter in which you have to test your luck
struct LuckChapter
{
  LuckChapter();

  ///Let the character undergo this chapter
  void Do(Character& character, const bool auto_play) const;

  const std::string& GetLuckText() const noexcept { return m_luck_text; }
  const std::string& GetNoLuckText() const noexcept { return m_no_luck_text; }
  int GetLuckChapter() const noexcept { return m_luck_chapter; }
  int GetNoLuckChapter() const noexcept { return m_no_luck_chapter; }

  void SetLuckText(const std::string& luck_text) noexcept { m_luck_text = luck_text; }
  void SetNoLuckText(const std::string& no_luck_text) noexcept { m_no_luck_text = no_luck_text; }
  void SetLuckChapter(const int luck_chapter) noexcept { m_luck_chapter = luck_chapter; }
  void SetNoLuckChapter(const int no_luck_chapter) noexcept { m_no_luck_chapter = no_luck_chapter; }

  private:

  int m_luck_chapter;
  std::string m_luck_text;
  int m_no_luck_chapter;
  std::string m_no_luck_text;

};

#endif // LUCKCHAPTER_H
