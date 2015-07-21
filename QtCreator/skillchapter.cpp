#include "skillchapter.h"

#include <sstream>

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

void SkillChapter::Do(Character& character, const ShowTextMode text_mode) const
{
  {
    std::stringstream s;
    s << "You test your skill..." << std::endl;
    ShowText(s.str(),text_mode);
    if (text_mode == ShowTextMode::debug ) { Wait(0.1); }
    if (text_mode == ShowTextMode::normal) { Wait(1.0); }
  }

  if (character.TestDexterity())
  {
    std::stringstream s;
    s << "Skill!\n"
      << GetSkillText() << std::endl
    ;
    ShowText(s.str(),text_mode);
    GetSkillConsequence().Apply(character);
  }
  else
  {
    std::stringstream s;
    s << "No skill!\n"
      << GetNoSkillText() << std::endl
    ;
    ShowText(s.str(),text_mode);
    GetNoSkillConsequence().Apply(character);
  }
}
