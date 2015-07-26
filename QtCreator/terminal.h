#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>
#include <vector>

#include "option.h"

struct Chapter;
struct Game;

///Shows the Game
struct Terminal
{
  Terminal();

  void ConnectTo(const Chapter& chapter);
  void ConnectTo(const Game& chapter);

  void SetAutoPlay(const bool auto_play) { m_auto_play = auto_play; }
  void SetSilent(const bool silent) { m_silent = silent; }

  void SetNumberOfCharsPerLine(const int n_chars) { m_n_chars = n_chars; }

  void SpeakText(const std::string& text);


  int SlotRequestInput(const std::vector<int>& valid_inputs);
  Option SlotRequestOption(const std::vector<Option>& valid_inputs);
  void SlotShowText(const std::string& text);
  void SlotWait();

  private:

  bool m_auto_play;

  ///Number of characters per line
  int m_n_chars;

  bool m_silent;

  ///Waiting time between characters in millisecs
  double m_wait_character_msec;

  ///Waiting time to increase the suspense
  double m_wait_suspense;
};

#endif // TERMINAL_H
