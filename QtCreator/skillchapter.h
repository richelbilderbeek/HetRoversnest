#ifndef SKILLCHAPTER_H
#define SKILLCHAPTER_H

#include <string>

struct Character;

///Chapter in which you have to test your luck
struct SkillChapter
{
  SkillChapter();

  ///Let the character undergo this chapter
  void Do(Character& character, const bool auto_play) const;

  const std::string& GetSkillText() const noexcept { return m_luck_text; }
  const std::string& GetNoSkillText() const noexcept { return m_no_luck_text; }
  int GetSkillChapter() const noexcept { return m_luck_chapter; }
  int GetNoSkillChapter() const noexcept { return m_no_luck_chapter; }

  void SetSkillText(const std::string& luck_text) noexcept { m_luck_text = luck_text; }
  void SetNoSkillText(const std::string& no_luck_text) noexcept { m_no_luck_text = no_luck_text; }
  void SetSkillChapter(const int luck_chapter) noexcept { m_luck_chapter = luck_chapter; }
  void SetNoSkillChapter(const int no_luck_chapter) noexcept { m_no_luck_chapter = no_luck_chapter; }

  private:

  int m_luck_chapter;
  std::string m_luck_text;
  int m_no_luck_chapter;
  std::string m_no_luck_text;

};

#endif // SKILLCHAPTER_H
