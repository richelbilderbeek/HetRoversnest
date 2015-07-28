#ifndef WALKTHROUGH_H
#define WALKTHROUGH_H

#include <string>
#include <vector>

#include "option.h"
#include "game.h"

struct Walkthrough
{
  Walkthrough(
    const int seed,
    const Character& character
  );

  void Start();

  private:

  Game m_game;

  Option SlotRequestOption(const std::vector<Option>& options);
  void SlotShowText(const std::string& text);
  void SlotWait();
};

#endif // WALKTHROUGH_H
