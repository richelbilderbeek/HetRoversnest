#include "option.h"

#include <algorithm>
#include <cassert>
#include "character.h"
#include <iostream>

Option::Option(
  const std::string& text,
  const Consequence& consequence
)
  : m_conditions{},
    m_consequence{consequence},
    m_text{text}
{
  assert(!text.empty());
  if (GetNextChapter() == 1)
  {
    std::cerr << "WARNING: next chapter is 1" << std::endl;
  }
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
  m_consequence.Apply(character);
}

std::ostream& operator<<(std::ostream& os, const Option& option)
{
  os
    << "Conditions: \n"
  ;
  for (const auto condition: option.GetConditions())
  {
    os << " * " << condition << '\n';
  }
  os
    << "m_next_chapter: " << option.GetNextChapter() << '\n'
    << "m_text: " << option.GetText() << '\n'
  ;
  return os;
}
