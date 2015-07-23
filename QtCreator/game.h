#ifndef GAME_H
#define GAME_H

#include "character.h"

#include <boost/signals2.hpp>

struct Game
{
  Game(
    const int rng_seed,
    const Character& character
  );

  void DoChapter();
  const Character& GetCharacter() const noexcept { return m_character; }
  bool HasLost() const noexcept { return m_has_lost; }
  bool HasWon() const noexcept { return m_has_won; }
  int GetCurrentChapterNumber() const noexcept;

  //If the Game wants an input
  mutable boost::signals2::signal<int(const std::vector<int>& valid_inputs)> m_signal_request_input;

  //If the Game want the dialog to display something
  mutable boost::signals2::signal<void(const std::string text)> m_signal_show_text;

  //If the Game wants the dialog to wait
  mutable boost::signals2::signal<void()> m_signal_wait;


  private:
  Character m_character;
  bool m_has_lost;
  bool m_has_won;

  int SlotRequestInput(const std::vector<int>& valid_inputs);
  void SlotShowText(const std::string& text);
  void SlotWait();

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

#endif // GAME_H
