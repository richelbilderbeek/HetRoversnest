#include "game.h"

#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

#include "character.h"
#include "helper.h"
#include "chapter.h"
#include "specialchapter.h"

Game::Game(
  const int rng_seed,
  const Character& character,
  const bool auto_play
)
  : m_character{character},
    m_has_lost{false},
    m_has_won{false}
{
  std::srand(rng_seed);

  while (1)
  {
    const int chapter_number{m_character.GetCurrentChapter()};
    if (chapter_number == 400)
    {
      m_has_won = true;
      break;
    }
    std::cout
      << '\n'
    #ifndef NDEBUG
      << std::string(60,'-') << '\n'
      << chapter_number << '\n'
      << std::string(60,'-') << '\n'
    #endif
    ;
    const Chapter chapter(chapter_number);
    chapter.Do(m_character,auto_play);
    if (m_character.IsDead())
    {
      m_has_lost = true;
      DoGameOver();
      return;
    }
  }
}
