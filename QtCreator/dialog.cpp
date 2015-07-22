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
    m_wait_suspense{1.0}
#else
    m_wait_character_msec{0.01},
    m_wait_suspense{0.5}
#endif
{

}

void Dialog::ConnectTo(const Chapter& chapter)
{
  chapter.m_signal_request_input.connect(
    boost::bind(&Dialog::SlotRequestInput,this)
  );
  chapter.m_signal_wait.connect(
    boost::bind(&Dialog::SlotWait,this)
  );
  chapter.m_signal_show_text.connect(
    boost::bind(&Dialog::SlotShowText,this,_1)
  );
}

std::string Dialog::SlotRequestInput()
{
  return "1";
}

void Dialog::SlotShowText(const std::string& text)
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

