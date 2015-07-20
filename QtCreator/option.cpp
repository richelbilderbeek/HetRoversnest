#include "option.h"

#include <cassert>
#include <iostream>

Option::Option(
  const std::string& text,
  const Consequence& consequence
)
  : m_condition{},
    m_consequence{consequence},
    m_text{text}
{
  assert(!text.empty());
  if (GetNextChapter() == 1)
  {
    std::cerr << "WARNING: next chapter is 1" << std::endl;
  }
}

bool Option::CanChoose(const Character& character) const
{
  return m_condition.IsSatisfied(character);
}

void Option::SetCondition(const Condition& condition)
{
  m_condition = condition;
}

std::ostream& operator<<(std::ostream& os, const Option& option)
{
  os
    << "Condition: " << option.GetCondition() << '\n'
    << "m_next_chapter: " << option.GetNextChapter() << '\n'
    << "m_text: " << option.GetText() << '\n'
  ;
  return os;
}
