#ifndef CHAPTER_H
#define CHAPTER_H

#include <string>
#include <vector>

#include "monster.h"

struct Chapter
{
  Chapter(const std::string& filename);

  auto GetNextChapter() const noexcept { return m_next_chapter; }
  const std::vector<Monster>& GetMonsters() const noexcept { return m_monsters; }
  auto GetText() const noexcept { return m_text; }

  private:
  std::vector<Monster> m_monsters;
  int m_next_chapter; //When there is no choice
  std::string m_text;
};

std::ostream& operator<<(std::ostream& os, const Chapter& chapter);

#endif // CHAPTER_H
