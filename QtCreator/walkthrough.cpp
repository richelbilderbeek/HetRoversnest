#include "walkthrough.h"

#include <iostream>

#include <boost/signals2.hpp>

#include "game.h"

Walkthrough::Walkthrough(
    const int seed,
    const Character& character
  ) : m_game(seed,character)
{
  m_game.m_signal_request_input.connect(
    boost::bind(&Walkthrough::SlotRequestInput,this,_1)
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
}

int Walkthrough::SlotRequestInput(const std::vector<int>& valid_inputs)
{
  return valid_inputs[1];
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
