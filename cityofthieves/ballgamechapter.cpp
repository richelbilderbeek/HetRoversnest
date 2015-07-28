#include "ballgamechapter.h"

#include <iostream>

#include "chapter.h"
#include "character.h"
#include "dice.h"
#include "helper.h"


BallGameChapter::BallGameChapter(Chapter& chapter)
  : m_chapter{chapter}
{

}

void BallGameChapter::Do(Character& character) const
{
  m_chapter.m_signal_show_text("The bare-chested man throws the ball to you.\n");
  m_chapter.m_signal_wait();

  while (1)
  {
    assert(Dice::Get());
    const int dice_you{Dice::Get()->Throw()};
    assert(dice_you >= 1 && dice_you <= 6);
    if (dice_you == 1)
    {
      m_chapter.m_signal_show_text("You drop the ball and pay the bare-chested man 5 gold pieces.\n");
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

    m_chapter.m_signal_show_text("You catch the ball and throw it to the bare-chested man.\n");
    m_chapter.m_signal_wait();

    const int dice_man{Dice::Get()->Throw()};
    assert(dice_man >= 1 && dice_man <= 6);
    if (dice_man == 1)
    {
      m_chapter.m_signal_show_text("The bare-chested man drops the ball and gives you 5 gold pieces.\n");
      character.ChangeGold(5);
      return;
    }

    m_chapter.m_signal_show_text("The bare-chested man catched the ball and throws it back to you.\n");
    m_chapter.m_signal_wait();
  }
}
