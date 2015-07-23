#include "walkthrough.h"

#include <iostream>

#include <boost/signals2.hpp>

#include "game.h"

Walkthrough::Walkthrough(
    const int seed,
    const Character& character
  ) : m_game(seed,character)
{
  m_game.m_signal_request_option.connect(
    boost::bind(&Walkthrough::SlotRequestOption,this,_1)
  );
  m_game.m_signal_wait.connect(
    boost::bind(&Walkthrough::SlotWait,this)
  );
  m_game.m_signal_show_text.connect(
    boost::bind(&Walkthrough::SlotShowText,this,_1)
  );
}

void Walkthrough::Start()
{
  while (1)
  {
    m_game.DoChapter();
    if (m_game.HasWon() || m_game.HasLost()) break;
  }
  assert(m_game.HasWon());
}

Option Walkthrough::SlotRequestOption(const std::vector<Option>& options)
{
  //If at [index], select option to go to [second]
  std::vector<int> solution(450,0);
  const int chapter_number{
    m_game.GetCurrentChapterNumber()
  };
  assert(solution[chapter_number] != 0);
  const int target_chapter{solution[chapter_number]};
  for (const auto option: options)
  {
    if (option.GetConsequence().GetNextChapter() == target_chapter)
    {
      return option;
    }
  }
  assert(!"Should not get here");
}

void Walkthrough::SlotShowText(const std::string& text)
{
  const int n_chars{60};
  int pos = 0;
  for (const char c: text)
  {
    if (c == '\n') pos = -1;
    else if (c == ' ' && pos > n_chars) { pos = 0; std::cout << '\n'; continue; }
    else if (c == ' ' && pos == 0) { continue; }
    std::cout << c;
    ++pos;
    std::cout.flush();
  }
}

void Walkthrough::SlotWait()
{
  //Continue
}
