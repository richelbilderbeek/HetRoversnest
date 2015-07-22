#include "dialog.h"

#include <cassert>
#include <fstream>
#include <iostream>

#include <boost/lambda/lambda.hpp>

#include "chapter.h"
#include "helper.h"

Dialog::Dialog()
  :
    n_chars{60},
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
  chapter.m_signal_request_input.connect(
    boost::bind(&Dialog::SlotRequestInput,this,_1)
  );
  chapter.m_signal_wait.connect(
    boost::bind(&Dialog::SlotWait,this)
  );
  chapter.m_signal_show_text.connect(
    boost::bind(&Dialog::SlotShowText,this,_1)
  );
}

void Dialog::DisconnectFrom(const Chapter& chapter)
{
  chapter.m_signal_request_input.disconnect(
    boost::bind(&Dialog::SlotRequestInput,this,_1)
  );
  chapter.m_signal_wait.disconnect(
    boost::bind(&Dialog::SlotWait,this)
  );
  chapter.m_signal_show_text.disconnect(
    boost::bind(&Dialog::SlotShowText,this,_1)
  );
}

int Dialog::SlotRequestInput(const std::vector<int> valid_inputs)
{
  #ifndef NDEBUG
  assert(valid_inputs.size() >= 2);
  //for (const int s: valid_inputs) { std::cerr << s << " "; }
  //std::cerr << std::endl;
  return valid_inputs[0] == 0 ? valid_inputs[1] : valid_inputs[0];
  #else
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
  #endif
}

void Dialog::SlotShowText(const std::string text)
{
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

