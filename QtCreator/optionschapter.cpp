#include "optionschapter.h"

#include <algorithm>
#include <cassert>

#include "character.h"

OptionsChapter::OptionsChapter()
  : m_options{}
{
}

void OptionsChapter::AddOption(const Option& option)
{
  m_options.push_back(option);
  std::random_shuffle(std::begin(m_options),std::end(m_options));
}

std::vector<Option> OptionsChapter::GetValidOptions(const Character& character) const noexcept
{
  assert(!character.IsDead());
  return m_options;
}
