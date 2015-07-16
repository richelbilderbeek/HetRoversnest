#include "option.h"

#include <cassert>
#include "character.h"

Option::Option(
  const std::string& text,
  const int next_chapter
)
  : m_next_chapter{next_chapter},
    m_text{text}
{
  assert(!text.empty());
  assert(next_chapter > 1);
}


void Option::DoChoose(Character& character) const
{
  character.SetChapter(m_next_chapter);
}
