#include "option.h"

#include <cassert>
#include "character.h"

Option::Option(
  const std::vector<std::string>& conditionals,
  const std::vector<std::string>& consequences,
  const std::string& text
)
  : m_conditionals{conditionals},
    m_consequences{consequences},
    m_text{text}
{
  assert(!text.empty());
}
