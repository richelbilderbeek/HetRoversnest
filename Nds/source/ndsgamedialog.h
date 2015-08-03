#ifndef NDSGAMEDIALOG_H
#define NDSGAMEDIALOG_H

#include <nds.h>

#include "observer.h"
#include "option.h"
#include "character.h"

namespace Ui { class NdsGameDialog; }

struct NdsGameDialog final : public Observer
{
  NdsGameDialog(const int argc, char* argv[]);

  void Start();

private:

  int m_key_pressed;


  std::vector<Option> m_options;

  PrintConsole m_screen_bottom;
  PrintConsole m_screen_top;

  void DoChapter();

  void CharacterChanged(const Character& character) override;
  void Wait() override;

  void ShowText(const std::string& text) override;
  Option RequestOption(const std::vector<Option>& options) override;

  //Check for key presses, touches, etceter
  void ProcessEvents();

  void UpdateStats();

  static const bool m_verbose{true};

};

#endif // NDSGAMEDIALOG_H