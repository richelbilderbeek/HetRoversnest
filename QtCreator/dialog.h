#ifndef DIALOG_H
#define DIALOG_H

#include <string>
#include <vector>

struct Chapter;

///Shows the Game
struct Dialog
{
  Dialog();

  void ConnectTo(const Chapter& chapter);
  void DisconnectFrom(const Chapter& chapter);

  void SpeakText(const std::string& text);


  int SlotRequestInput(const std::vector<int> valid_inputs);
  void SlotShowText(const std::string text);
  void SlotWait();

  private:

  ///Number of characters per line
  int n_chars;

  ///Waiting time between characters in millisecs
  double m_wait_character_msec;

  ///Waiting time to increase the suspense
  double m_wait_suspense;
};

#endif // DIALOG_H
