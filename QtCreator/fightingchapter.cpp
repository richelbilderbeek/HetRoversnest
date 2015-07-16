#include "fightingchapter.h"

#include <cassert>

FightingChapter::FightingChapter()
  : m_escape_chapter{-1},
    m_fight_sequentially{true},
    m_monsters{},
    m_rounds_to_escape{1000}
{

}

void FightingChapter::AddMonster(const Monster& monster)
{
  m_monsters.push_back(monster);
}

void FightingChapter::SetEscapeToChapter(const int escape_to_chapter)
{
  m_escape_chapter = escape_to_chapter;
  assert(m_escape_chapter > 0);

}

void FightingChapter::SetRoundsToEscape(const int rounds_to_escape)
{
  m_rounds_to_escape = rounds_to_escape;
  assert(m_rounds_to_escape >= 0);
}
