#include "game.h"

#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

#include "ai.h"
#include "character.h"
#include "dice.h"
#include "helper.h"
#include "chapter.h"
#include "specialchapter.h"

Game::Game(
  const int rng_seed,
  const Character& character
)
  : m_character{character},
    m_has_lost{false},
    m_has_won{false}
{
  Dice::Get()->SetSeed(rng_seed);

  #ifndef NDEBUG
  {
    std::ofstream f("last_seed.txt");
    f << rng_seed;
  }
  #endif
}

void Game::DoChapter()
{
  if (m_has_lost || m_has_won) return;

  const int chapter_number{m_character.GetCurrentChapter()};
  if (chapter_number == 400)
  {
    m_has_won = true;
    return;
  }


  const Chapter chapter(chapter_number);
  chapter.Do(m_character);

  if (m_character.IsDead()) { m_has_lost = true; }
}
