#include "luckchapter.h"

#include <sstream>

#include "chapter.h"
#include "character.h"
#include "helper.h"

LuckChapter::LuckChapter(Chapter& chapter)
  :
    m_chapter{chapter},
    m_luck_consequence{},
    m_luck_text{},
    m_no_luck_consequence{},
    m_no_luck_text{}
{

}

void LuckChapter::Do(Character& character) const
{
  m_chapter.m_signal_show_text("You test your luck...\n");
  m_chapter.m_signal_wait();

  if (character.TestLuck())
  {
    m_chapter.m_signal_show_text("Luck!\n" + GetLuckText() + "\n");
    character.SetChapter(GetLuckChapter());
  }
  else
  {
    m_chapter.m_signal_show_text("No luck!\n" + GetNoLuckText() + "\n");
    character.SetChapter(GetNoLuckChapter());
  }
}

void LuckChapter::SetLuckChapter(const int luck_chapter) noexcept
{
  m_luck_consequence.SetNextChapter(luck_chapter);
}

void LuckChapter::SetNoLuckChapter(const int no_luck_chapter) noexcept
{
  m_no_luck_consequence.SetNextChapter(no_luck_chapter);
}
