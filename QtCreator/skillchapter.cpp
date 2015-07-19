#include "skillchapter.h"

#include <cassert>
#include <sstream>

#include "consequence.h"
#include "character.h"
#include "helper.h"
SkillChapter::SkillChapter()
  :
    m_skill_consequence{},
    m_skill_text{},
    m_no_skill_consequence{},
    m_no_skill_text{}
{

}

void SkillChapter::Do(Character& character, const bool auto_play) const
{
  {
    std::stringstream s;
    s << "You test your skill..." << std::endl;
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
    GetSkillConsequence().Apply(character);
  }
  else
  {
    std::stringstream s;
    s << "No skill!\n"
      << GetNoSkillText() << std::endl
    ;
    ShowText(s.str(),auto_play);
    GetNoSkillConsequence().Apply(character);
  }
}
