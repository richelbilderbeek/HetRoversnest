#include "dialog.h"

#include <cassert>
#include <fstream>
#include <iostream>

#include <boost/lambda/lambda.hpp>

#include "chapter.h"
#include "game.h"
#include "helper.h"

Dialog::Dialog()
  :
    m_auto_play{false},
    n_chars{60},
    m_silent{false},
#ifndef NDEBUG
    m_wait_character_msec{0.0},
    m_wait_suspense{0.0}
#else
    m_wait_character_msec{0.01},
    m_wait_suspense{0.5}
#endif
{

}

void Dialog::ConnectTo(const Chapter& chapter)
{
  chapter.m_signal_request_option.connect(
    boost::bind(&Dialog::SlotRequestOption,this,_1)
  );
  chapter.m_signal_wait.connect(
    boost::bind(&Dialog::SlotWait,this)
  );
  chapter.m_signal_show_text.connect(
    boost::bind(&Dialog::SlotShowText,this,_1)
  );
}

void Dialog::ConnectTo(const Game& game)
{
  game.m_signal_request_option.connect(
    boost::bind(&Dialog::SlotRequestOption,this,_1)
  );
  game.m_signal_wait.connect(
    boost::bind(&Dialog::SlotWait,this)
  );
  game.m_signal_show_text.connect(
    boost::bind(&Dialog::SlotShowText,this,_1)
  );
}

Option Dialog::SlotRequestOption(const std::vector<Option>& options)
{
  assert(!options.empty());
  const int n_options{static_cast<int>(options.size())};

  while (1)
  {
    std::vector<int> valid_indices;
    std::stringstream text;
    for (int i=0; i!=n_options; ++i)
    {
      valid_indices.push_back(i);
      text << "[" << i << "] " << options[i].GetText() << '\n';
    }
    SlotShowText(text.str());
    const int chosen_index{SlotRequestInput(valid_indices)};
    assert(chosen_index >= 0);
    assert(chosen_index < n_options);
    return options[chosen_index];
  }
}

int Dialog::SlotRequestInput(const std::vector<int>& valid_inputs)
{
  assert(!valid_inputs.empty());
  if (valid_inputs.size() == 1) { return valid_inputs[0]; }
  if (m_auto_play)
  {
    return valid_inputs[0] == 0 ? valid_inputs[1] : valid_inputs[0];
  }
  else
  {
    while (1)
    {
      std::string s;
      std::getline(std::cin,s);
      if (!IsInt(s))
      {
        SlotShowText("Please enter an integer\n");
        continue;
      }
      const int i{std::stoi(s)};
      const auto iter = std::find(std::begin(valid_inputs),std::end(valid_inputs),i);
      if (iter == std::end(valid_inputs))
      {
        std::stringstream t;
        std::copy(std::begin(valid_inputs),std::end(valid_inputs),std::ostream_iterator<int>(t,", "));
        std::string u{t.str()};
        u.pop_back(); u.pop_back(); //Pop the tailing comma and space
        SlotShowText("Please enter a valid input: " + u + "\n");
        continue;
      }
      return *iter;
    }
  }
}

void Dialog::SlotShowText(const std::string& text)
{
  if (m_silent) return;
  int pos = 0;
  for (const char c: text)
  {
    if (c == '\n') pos = -1;
    else if (c == ' ' && pos > n_chars) { pos = 0; std::cout << '\n'; continue; }
    else if (c == ' ' && pos == 0) { continue; }
    std::cout << c;
    ++pos;
    std::cout.flush();
    Wait(m_wait_character_msec);
  }
}

void Dialog::SlotWait()
{
  Wait(m_wait_suspense);
}

void Dialog::SpeakText(const std::string& text)
{
  std::ofstream f("espeak.txt");
  f << text;
  f.close();
  const int error{std::system("espeak -f espeak.txt")};
  assert(!error);
  if (error) {;}
}
