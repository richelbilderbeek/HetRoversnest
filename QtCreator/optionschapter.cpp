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
  std::vector<Option> valid_options;
  std::copy_if(
    std::begin(m_options),
    std::end(m_options),
    std::back_inserter(valid_options),
    [character](const Option& option)
    {
      if (option.GetGoldNeeded() > character.GetGold()) return false;
      for (const auto item_needed: option.GetItemsNeeded())
      {
        if (!character.HasItem(item_needed)) return false;
      }
      for (const auto item_not_needed: option.GetItemsNotNeeded())
      {
        if (character.HasItem(item_not_needed)) return false;
      }
      return true;
    }
  );
  return valid_options;
}
