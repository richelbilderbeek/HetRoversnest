#ifndef MENUDIALOG_H
#define MENUDIALOG_H

#include <boost/signals2.hpp>

#include "option.h"

struct MenuDialog
{
  MenuDialog();

  void Execute();

  //If the MenuDialog changed the character
  mutable boost::signals2::signal<void(const Character&)> m_signal_character_has_changed;

  //If the MenuDialog wants an input
  mutable boost::signals2::signal<Option(const std::vector<Option>& valid_option)> m_signal_request_option;

  //If the MenuDialog want the dialog to display something
  mutable boost::signals2::signal<void(const std::string& text)> m_signal_show_text;

  //If the MenuDialog wants the dialog to wait
  mutable boost::signals2::signal<void()> m_signal_wait;

  void ShowZanbarBone();

  private:

  Character CreateCharacter() const noexcept;

  std::string GetAboutText() const noexcept { return "About"; }
  std::string GetCreateGraphText() const noexcept { return "Create graph"; }
  std::string GetHintsText() const noexcept { return "Hints"; }
  std::string GetIntroductionText() const noexcept { return "Introduction"; }
  std::string GetManualText() const noexcept { return "Manual"; }
  std::vector<Option> GetMenuOptions() const noexcept;
  std::string GetQuitText() const noexcept { return "Quit"; }
  std::string GetSolveGameText() const noexcept { return "Solve game"; }
  std::string GetStartGameText() const noexcept { return "Start game"; }
  std::string GetTeaserText() const noexcept { return "Teaser"; }

  void ShowAbout();
  void ShowIntroduction();
  void ShowManual();
  void ShowTeaser();
  void ShowHints();

  void SlotCharacterChanged(const Character& character);
  Option SlotRequestOption(const std::vector<Option>& valid_inputs);
  void SlotShowText(const std::string& text);
  void SlotWait();

  void StartGame();

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

#endif // MENUDIALOG_H
