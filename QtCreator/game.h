#ifndef GAME_H
#define GAME_H

#include "character.h"

struct Game
{
  Game(
    const int rng_seed,
    const Character& character,
    const bool auto_play
  );

  const Character& GetCharacter() const noexcept { return m_character; }
  bool HasLost() const noexcept { return m_has_lost; }
  bool HasWon() const noexcept { return m_has_won; }

  private:
  Character m_character;
  bool m_has_lost;
  bool m_has_won;

};

#endif // GAME_H
