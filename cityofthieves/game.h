#ifndef GAME_H
#define GAME_H

#include <boost/signals2.hpp>

#include "character.h"
#include "option.h"


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

  #ifndef NDEBUG
  void SetChapter(const int chapter);
  #endif

  //If the Game changed the character
  mutable boost::signals2::signal<void(const Character&)> m_signal_character_has_changed;

  //If the Game wants an Option
  mutable boost::signals2::signal<Option(const std::vector<Option>& valid_options)> m_signal_request_option;

  //If the Game want the dialog to display something
  mutable boost::signals2::signal<void(const std::string text)> m_signal_show_text;

  //If the Game wants the dialog to wait
  mutable boost::signals2::signal<void()> m_signal_wait;


  private:
  Character m_character;
  bool m_has_lost;
  bool m_has_won;

  void SlotCharacterChanged(const Character& character);
  Option SlotRequestOption(const std::vector<Option>& valid_inputs);
  void SlotShowText(const std::string& text);
  void SlotWait();

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

#endif // GAME_H
