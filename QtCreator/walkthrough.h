#ifndef WALKTHROUGH_H
#define WALKTHROUGH_H

#include <string>
#include <vector>

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

  int SlotRequestInput(const std::vector<int>& valid_inputs);
  void SlotShowText(const std::string& text);
  void SlotWait();
};

#endif // WALKTHROUGH_H
