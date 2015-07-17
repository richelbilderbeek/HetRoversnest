#include "skillchapter.h"

#include <cassert>
#include <sstream>

#include "character.h"
#include "helper.h"
SkillChapter::SkillChapter()
  :
    m_luck_chapter{},
    m_luck_text{},
    m_no_luck_chapter{},
    m_no_luck_text{}
{

}

void SkillChapter::Do(Character& character, const bool auto_play) const
{
  {
    std::stringstream s;
    s << "You test your luck..." << std::endl;
    ShowText(s.str(),auto_play);
    if (!auto_play) { Wait(1.0); }
  }

  if (character.TestDexterity())
  {
    std::stringstream s;
    s << "Skill!\n"
      << GetSkillText() << std::endl
    ;
    ShowText(s.str(),auto_play);
    character.SetChapter(GetSkillChapter());
  }
  else
  {
    std::stringstream s;
    s << "No skill!\n"
      << GetNoSkillText() << std::endl
    ;
    ShowText(s.str(),auto_play);
    character.SetChapter(GetNoSkillChapter());
  }
}
