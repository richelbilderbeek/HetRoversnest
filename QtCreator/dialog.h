#ifndef DIALOG_H
#define DIALOG_H

#include <string>
#include <vector>

struct Chapter;
struct Game;

///Shows the Game
struct Dialog
{
  Dialog();

  void ConnectTo(const Chapter& chapter);
  void ConnectTo(const Game& chapter);

  void SetAutoPlay(const bool auto_play) { m_auto_play = auto_play; }
  void SetSilent(const bool silent) { m_silent = silent; }
  void SpeakText(const std::string& text);


  int SlotRequestInput(const std::vector<int>& valid_inputs);
  void SlotShowText(const std::string& text);
  void SlotWait();

  private:

  bool m_auto_play;

  ///Number of characters per line
  int n_chars;

  bool m_silent;

  ///Waiting time between characters in millisecs
  double m_wait_character_msec;

  ///Waiting time to increase the suspense
  double m_wait_suspense;
};

#endif // DIALOG_H
