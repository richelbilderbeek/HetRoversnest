#include "luckchapter.h"

#include <sstream>

#include "character.h"
#include "helper.h"

LuckChapter::LuckChapter()
  :
    m_luck_consequence{},
    m_luck_text{},
    m_no_luck_consequence{},
    m_no_luck_text{}
{

}

void LuckChapter::Do(Character& character, const ShowTextMode text_mode) const
{
  {
    std::stringstream s;
    s << "You test your luck..." << std::endl;
    ShowText(s.str(),text_mode);
    if (text_mode == ShowTextMode::debug ) { Wait(0.1); }
    if (text_mode == ShowTextMode::normal) { Wait(1.0); }
  }

  if (character.TestLuck())
  {
    std::stringstream s;
    s << "Luck!\n"
      << GetLuckText() << std::endl
    ;
    ShowText(s.str(),text_mode);
    character.SetChapter(GetLuckChapter());
  }
  else
  {
    std::stringstream s;
    s << "No luck!\n"
      << GetNoLuckText() << std::endl
    ;
    ShowText(s.str(),text_mode);
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
