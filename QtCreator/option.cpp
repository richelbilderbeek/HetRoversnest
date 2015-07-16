#include "option.h"

#include <algorithm>
#include <cassert>
#include "character.h"

Option::Option(
  const std::string& text,
  const int next_chapter
)
  : m_gold_needed{0},
    m_items_needed{},
    m_items_not_needed{},
    m_next_chapter{next_chapter},
    m_text{text}
{
  assert(!text.empty());
  assert(next_chapter > 1);
}

void Option::AddItemNeeded(const Item item)
{
  assert(
    std::count(
      std::begin(m_items_needed),
      std::end(m_items_needed),
      item
    ) == 0
  );
  m_items_needed.push_back(item);
}

void Option::AddItemNotNeeded(const Item item)
{
  assert(
    std::count(
      std::begin(m_items_not_needed),
      std::end(m_items_not_needed),
      item
    ) == 0
  );
  m_items_not_needed.push_back(item);
}


void Option::DoChoose(Character& character) const
{
  character.SetChapter(m_next_chapter);
}

void Option::SetGoldNeeded(const int gold_needed)
{
  assert(gold_needed > 0);
  m_gold_needed = gold_needed;
}
