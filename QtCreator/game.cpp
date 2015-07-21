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
  const Character& character,
  Ai * const ai
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


  while (1)
  {
    const int chapter_number{m_character.GetCurrentChapter()};
    if (chapter_number == 400)
    {
      m_has_won = true;
      break;
    }

    #ifndef NDEBUG
    const ShowTextMode text_mode{
      ai ? ShowTextMode::silent : ShowTextMode::debug
    };
    #else
    const ShowTextMode text_mode{ShowTextMode::normal};
    #endif

    if (text_mode == ShowTextMode::debug)
    {
      std::cout << '\n'
        << std::string(60,'-') << '\n'
        << chapter_number << '\n'
        << std::string(60,'-') << '\n'
      ;
    }
    const Chapter chapter(chapter_number);
    chapter.Do(m_character,text_mode,ai);
    if (m_character.IsDead())
    {
      m_has_lost = true;
      DoGameOver(text_mode);
      return;
    }
  }
}
