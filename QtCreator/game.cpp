#include "game.h"

#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

#include "ai.h"
#include "character.h"
#include "dialog.h"
#include "dice.h"
#include "helper.h"
#include "chapter.h"
#include "specialchapter.h"

Game::Game(
  const int rng_seed,
  const Character& character
)
  :
    m_signal_request_input{},
    m_signal_request_option{},
    m_signal_show_text{},
    m_signal_wait{},
    m_character{character},
    m_has_lost{false},
    m_has_won{false}

{
  #ifndef NDEBUG
  Test();
  #endif
  Dice::Get()->SetSeed(rng_seed);
}

void Game::DoChapter()
{
  assert(m_signal_request_input.num_slots() > 0);
  assert(m_signal_wait.num_slots() > 0);
  assert(m_signal_request_input.num_slots() > 0);

  if (m_has_lost || m_has_won) return;

  const int chapter_number{GetCurrentChapterNumber()};
  if (chapter_number == 400)
  {
    m_has_won = true;
    return;
  }

  const Chapter chapter(chapter_number);

  chapter.m_signal_request_input.connect(
    boost::bind(&Game::SlotRequestInput,this,_1)
  );
  chapter.m_signal_request_option.connect(
    boost::bind(&Game::SlotRequestOption,this,_1)
  );
  chapter.m_signal_wait.connect(
    boost::bind(&Game::SlotWait,this)
  );
  chapter.m_signal_show_text.connect(
    boost::bind(&Game::SlotShowText,this,_1)
  );
  chapter.Do(m_character);

  if (m_character.IsDead()) { m_has_lost = true; }
}

int Game::GetCurrentChapterNumber() const noexcept
{
  return m_character.GetCurrentChapter();
}

int Game::SlotRequestInput(const std::vector<int>& valid_inputs)
{
  return *m_signal_request_input(valid_inputs);
}

Option Game::SlotRequestOption(const std::vector<Option>& valid_options)
{
  return *m_signal_request_option(valid_options);
}

void Game::SlotShowText(const std::string& text)
{
  m_signal_show_text(text);
}

void Game::SlotWait()
{
  m_signal_wait();
}

#ifndef NDEBUG
void Game::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  TestHelperFunctions();
  Chapter(1);

  //Test if game runs identically twice with same RNG seed
  {
    const int seed{123456};
    Dice::Get()->SetSeed(seed);

    Character character1(
      Dice::Get()->Throw(),
      Dice::Get()->Throw() + Dice::Get()->Throw(),
      Dice::Get()->Throw(),
      Item::luck_potion
    );

    const Game game1(seed,character1);

    Dice::Get()->SetSeed(seed);

    Character character2(
      Dice::Get()->Throw(),
      Dice::Get()->Throw() + Dice::Get()->Throw(),
      Dice::Get()->Throw(),
      Item::luck_potion
    );

    const Game game2(seed,character2);
    assert(character1.GetChapters() == character2.GetChapters());
  }
}
#endif
