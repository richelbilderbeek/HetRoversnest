#include "option.h"

#include <algorithm>
#include <cassert>
#include "character.h"

Option::Option(
  const std::string& text,
  const int next_chapter
)
  : m_conditions{},
    m_next_chapter{next_chapter},
    m_text{text}
{
  assert(!text.empty());
  assert(next_chapter > 1);
}

void Option::AddCondition(const Condition& condition)
{
  m_conditions.push_back(condition);
}

bool Option::CanChoose(const Character& character) const
{
  for (const auto condition: m_conditions)
  {
    if (!condition.IsSatisfied(character)) return false;
  }
  return true;
}

void Option::DoChoose(Character& character) const
{
  character.SetChapter(m_next_chapter);
}
