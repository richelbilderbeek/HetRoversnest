#include "specialchapter.h"

#include <iostream>
#include <sstream>

#include "character.h"
#include "helper.h"

void DoPlayBall(Character& character, const bool auto_play)
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
    ShowText(s.str(),auto_play);
  }
  if (!auto_play) { Wait(0.5); }

  while (1)
  {
    const int dice_you{1 + ((std::rand() >> 4) % 6)};
    if (dice_you == 1)
    {
      std::stringstream s;
      s << "You drop the ball and pay the bare-chested man 5 gold pieces.\n";
      ShowText(s.str(),auto_play);
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
      ShowText(s.str(),auto_play);
    }
    if (!auto_play) { Wait(0.5); }
    const int dice_man{1 + ((std::rand() >> 4) % 6)};
    if (dice_man == 1)
    {
      std::stringstream s;
      s << "The bare-chested man drops the ball and gives you 5 gold pieces.\n";
      ShowText(s.str(),auto_play);
      character.ChangeGold(5);
      return;
    }
    {
      std::stringstream s;
      s << "The bare-chested man catched the ball and throws it back to you.\n";
      ShowText(s.str(),auto_play);
    }
    if (!auto_play) { Wait(0.5); }
  }
}

void DoPlayDice(Character& character, const bool auto_play)
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
    ShowText(s.str(),auto_play);
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
      ShowText(s.str(),auto_play);
    }
    std::string s;
    if (!auto_play)
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
      ShowText(s.str(),auto_play);
    }
  }


  const int n_rounds{4};
  for (int round = 0; round != 4; ++round)
  {

    {
      std::stringstream s;
      s << "Round #" << (round + 1) << "/" << n_rounds << '\n';
      ShowText(s.str(),auto_play);
    }
    //You
    const int dice_you = 1 + ((std::rand() >> 4) % 6);
    const int dice_1 = 1 + ((std::rand() >> 4) % 6);
    const int dice_2 = 1 + ((std::rand() >> 4) % 6);
    const int dice_3 = 1 + ((std::rand() >> 4) % 6);
    {
      std::stringstream s;
      s << "You throw: " << dice_you << '\n';
      ShowText(s.str(),auto_play);
      if (!auto_play) { Wait(0.5); }
    }
    {
      std::stringstream s;
      s << "Dwarf 1 throws: " << dice_1 << '\n';
      ShowText(s.str(),auto_play);
      if (!auto_play) { Wait(0.5); }
    }
    {
      std::stringstream s;
      s << "Dwarf 2 throws: " << dice_2 << '\n';
      ShowText(s.str(),auto_play);
      if (!auto_play) { Wait(0.5); }
    }
    {
      std::stringstream s;
      s << "Dwarf 3 throws: " << dice_3 << '\n';
      ShowText(s.str(),auto_play);
      if (!auto_play) { Wait(0.5); }
    }
    if (dice_you > dice_1 && dice_you > dice_2 && dice_you > dice_3)
    {
      std::stringstream s;
      s << "You won! You collect your 6 gold pieces from the three dwarves.\n";
      ShowText(s.str(),auto_play);
      character.ChangeGold(6);
      if (!auto_play) { Wait(0.5); }
    }
    else if (dice_you < dice_1 || dice_you < dice_2 || dice_you < dice_3)
    {
      std::stringstream s;
      s << "You Lost! You lose 2 gold pieces to the three dwarves.\n";
      ShowText(s.str(),auto_play);
      character.ChangeGold(-2);
      if (!auto_play) { Wait(0.5); }
    }
    else
    {
      std::stringstream s;
      s << "A draw. You keep your gold\n";
      ShowText(s.str(),auto_play);
      if (!auto_play) { Wait(0.5); }
    }

    if (character.GetGold() < 2)
    {
      std::stringstream s;
      s << "You cannot afford to play another round.";
      ShowText(s.str(),auto_play);
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
        ShowText(s.str(),auto_play);
      }
      std::string s;
      if (!auto_play)
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
        ShowText(s.str(),auto_play);
      }
    }
  }
}

void DoPlayPill(Character& character, const bool auto_play)
{
  const int dice{1 + ((std::rand() >> 4) % 6)};
  if (dice == 1)
  {
    std::stringstream s;
    s << "You die quickly from the poisoned pill and your adventure ends here.\n";
    ShowText(s.str(),auto_play);
    character.SetIsDead();
  }
}


void DoGameOver()
{
  std::cout
    << '\n'
    << "*************\n"
    << "*           *\n"
    << "* GAME OVER *\n"
    << "*           *\n"
    << "*************\n"
  ;
}

void DoGameWon()
{
  std::cout
    << '\n'
    << "*************\n"
    << "*           *\n"
    << "* GAME WON! *\n"
    << "*           *\n"
    << "*************\n"
  ;
}
