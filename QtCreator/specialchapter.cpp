#include "specialchapter.h"

#include <cassert>
#include <iostream>
#include <sstream>

#include "character.h"
#include "dice.h"
#include "helper.h"

void DoPlayBall(Character& character, const ShowTextMode text_mode)
{
  /*
Roll one die alternately for yourself and for the
bare-chested man, to represent the cannon-fcuH pas-
sing between you. Repeat this process until a 1 is
thrown, in which case the cannon-tall has
been dropped and the loser must pay the winner 5
Gold Pieces.
  */

  {
    std::stringstream s;
    s << "The bare-chested man throws the ball to you.\n";
    ShowText(s.str(),text_mode);
  }
  if (text_mode == ShowTextMode::normal) { Wait(0.50); }
  if (text_mode == ShowTextMode::debug ) { Wait(0.05); }

  while (1)
  {
    assert(Dice::Get());
    const int dice_you{Dice::Get()->Throw()};
    assert(dice_you >= 1 && dice_you <= 6);
    if (dice_you == 1)
    {
      std::stringstream s;
      s << "You drop the ball and pay the bare-chested man 5 gold pieces.\n";
      ShowText(s.str(),text_mode);
      if (character.GetGold() >= 5)
      {
        character.ChangeGold(-5);
      }
      else
      {
        std::clog << "WARNING: should not have got here, due to lack of gold\n";
        character.ChangeGold(-character.GetGold());
      }
      return;
    }
    {
      std::stringstream s;
      s << "You catch the ball and throw it to the bare-chested man.\n";
      ShowText(s.str(),text_mode);
    }

    if (text_mode == ShowTextMode::normal) { Wait(0.50); }
    if (text_mode == ShowTextMode::debug ) { Wait(0.05); }

    const int dice_man{Dice::Get()->Throw()};
    assert(dice_man >= 1 && dice_man <= 6);
    if (dice_man == 1)
    {
      std::stringstream s;
      s << "The bare-chested man drops the ball and gives you 5 gold pieces.\n";
      ShowText(s.str(),text_mode);
      character.ChangeGold(5);
      return;
    }
    {
      std::stringstream s;
      s << "The bare-chested man catched the ball and throws it back to you.\n";
      ShowText(s.str(),text_mode);
    }
    if (text_mode == ShowTextMode::normal) { Wait(0.50); }
    if (text_mode == ShowTextMode::debug ) { Wait(0.05); }
  }
}

void DoPlayDice(Character& character, const ShowTextMode text_mode)
{
  /*

The stakes are 2 gold
pieces. Each person will stake 2 gold pieces and roll
two dice. The person rolling the highest number
collects the 8 gold pieces. To play, roll two dice
three limes for the three Dwarfs and make a note of
each total. Next, roll two dice for yourself. If your
own total is higher than each of the other three
totals, you win 6 gold pieces from the Dwarfs. If the
total is' less than any of the other three totals, you
lose 2 Gold Pieces. You may play four times if you
wish to

  */
  if (character.GetGold() < 2)
  {
    std::stringstream s;
    s << "You cannot afford to play this game.";
    ShowText(s.str(),text_mode);
    return;
  }

  while (1)
  {
    {
      std::stringstream s;
      s
        << "Do you want to play?\n"
        << "[1] Yes\n"
        << "[2] No\n"
      ;
      ShowText(s.str(),text_mode);
    }
    std::string s;
    if (text_mode == ShowTextMode::debug || text_mode == ShowTextMode::normal)
    {
      std::getline(std::cin,s);
    }
    else
    {
      switch ((std::rand() >> 4) % 3)
      {
        case 0: s = "1"; break;
        case 1: s = "2"; break;
        case 2: s = "x"; break;
      }
    }
    if (s == "2") { return; }
    if (s == "1") { break; }
    {
      std::stringstream s;
      s
        << "Please enter either '1' or '2'.\n"
      ;
      ShowText(s.str(),text_mode);
    }
  }


  const int n_rounds{4};
  for (int round = 0; round != 4; ++round)
  {

    {
      std::stringstream s;
      s << "Round #" << (round + 1) << "/" << n_rounds << '\n';
      ShowText(s.str(),text_mode);
    }
    //You
    const int dice_you{Dice::Get()->Throw()};
    const int dice_1{Dice::Get()->Throw()};
    const int dice_2{Dice::Get()->Throw()};
    const int dice_3{Dice::Get()->Throw()};
    {
      std::stringstream s;
      s << "You throw: " << dice_you << '\n';
      ShowText(s.str(),text_mode);
      if (text_mode == ShowTextMode::normal) { Wait(0.50); }
      if (text_mode == ShowTextMode::debug ) { Wait(0.05); }
    }
    {
      std::stringstream s;
      s << "Dwarf 1 throws: " << dice_1 << '\n';
      ShowText(s.str(),text_mode);
      if (text_mode == ShowTextMode::normal) { Wait(0.50); }
      if (text_mode == ShowTextMode::debug ) { Wait(0.05); }
    }
    {
      std::stringstream s;
      s << "Dwarf 2 throws: " << dice_2 << '\n';
      ShowText(s.str(),text_mode);
      if (text_mode == ShowTextMode::normal) { Wait(0.50); }
      if (text_mode == ShowTextMode::debug ) { Wait(0.05); }
    }
    {
      std::stringstream s;
      s << "Dwarf 3 throws: " << dice_3 << '\n';
      ShowText(s.str(),text_mode);
      if (text_mode == ShowTextMode::normal) { Wait(0.50); }
      if (text_mode == ShowTextMode::debug ) { Wait(0.05); }
    }
    if (dice_you > dice_1 && dice_you > dice_2 && dice_you > dice_3)
    {
      std::stringstream s;
      s << "You won! You collect your 6 gold pieces from the three dwarves.\n";
      ShowText(s.str(),text_mode);
      character.ChangeGold(6);
      if (text_mode == ShowTextMode::normal) { Wait(0.50); }
      if (text_mode == ShowTextMode::debug ) { Wait(0.05); }
    }
    else if (dice_you < dice_1 || dice_you < dice_2 || dice_you < dice_3)
    {
      std::stringstream s;
      s << "You Lost! You lose 2 gold pieces to the three dwarves.\n";
      ShowText(s.str(),text_mode);
      character.ChangeGold(-2);
      if (text_mode == ShowTextMode::normal) { Wait(0.50); }
      if (text_mode == ShowTextMode::debug ) { Wait(0.05); }
    }
    else
    {
      std::stringstream s;
      s << "A draw. You keep your gold\n";
      ShowText(s.str(),text_mode);
      if (text_mode == ShowTextMode::normal) { Wait(0.50); }
      if (text_mode == ShowTextMode::debug ) { Wait(0.05); }
    }

    if (character.GetGold() < 2)
    {
      std::stringstream s;
      s << "You cannot afford to play another round.";
      ShowText(s.str(),text_mode);
      break;
    }

    if (round == 3) break;

    //Play again?
    while (1)
    {
      {
        std::stringstream s;
        s
          << "Do you want to play another round?\n"
          << "[1] Yes\n"
          << "[2] No\n"
        ;
        ShowText(s.str(),text_mode);
      }
      std::string s;
      if (text_mode == ShowTextMode::normal || text_mode == ShowTextMode::debug)
      {
        std::getline(std::cin,s);
      }
      else
      {
        switch (Dice::Get()->Throw())
        {
          case 1: case 2: s = "1"; break;
          case 3: case 4: case 5: s = "2"; break;
          case 6: s = "x"; break;
        }
      }
      if (s == "2") { return; }
      if (s == "1") { break; }
      {
        std::stringstream s;
        s
          << "Please enter either '1' or '2'.\n"
        ;
        ShowText(s.str(),text_mode);
      }
    }
  }
}

void DoPlayPill(Character& character, const ShowTextMode text_mode)
{
  const int dice{1 + ((std::rand() >> 4) % 6)};
  if (dice == 1)
  {
    std::stringstream s;
    s << "You die quickly from the poisoned pill and your adventure ends here.\n";
    ShowText(s.str(),text_mode);
    character.SetIsDead();
  }
}


void DoGameOver(const ShowTextMode text_mode)
{
  std::stringstream s;
  s
    << '\n'
    << "*************\n"
    << "*           *\n"
    << "* GAME OVER *\n"
    << "*           *\n"
    << "*************\n"
  ;
  ShowText(s.str(),text_mode);
}

void DoGameWon(const ShowTextMode text_mode)
{
  std::stringstream s;
  s
    << '\n'
    << "*************\n"
    << "*           *\n"
    << "* GAME WON! *\n"
    << "*           *\n"
    << "*************\n"
  ;
  ShowText(s.str(),text_mode);
}
