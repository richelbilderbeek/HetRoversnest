#include "ndsgamedialog.h"

#include <cassert>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <nds.h>
#include <stdio.h>

#include "chapter.h"
#include "menudialog.h"
#include "helper.h"
#include "dice.h"


NdsGameDialog::NdsGameDialog(const int argc, char* argv[])
  : m_key_pressed{-1},
    m_options{},
    m_screen_bottom{},
    m_screen_top{}
{


  consoleInit(&m_screen_top,    3,BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
  consoleInit(&m_screen_bottom, 3,BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);


  consoleSelect(&m_screen_top);

  const Helper h;

  if (m_verbose) { h.CoutNl("Console mode initialized"); }

  if (m_verbose)
  {
    h.CoutNl("Test waiting: wait 1 second");
    h.Wait(1.0);
  }

  if (m_verbose)
  {
    h.Cout("Displaying the character x:"); h.Cout('x'); h.Cout('\n');
    h.Cout("Displaying the character Y:"); h.Cout('Y'); h.Cout('\n');
    h.Cout("Displaying the character 0:"); h.Cout('0'); h.Cout('\n');
  }

  if (m_verbose)
  {
    std::stringstream s;
    s << "argc:" << argc << '\n'
      << "argv[0]:" << std::string(argv[0]) << '\n'
    ;
    h.CoutNl(s.str());
  }

  if (m_verbose) { h.CoutNl("Done initializing game"); }
}


void NdsGameDialog::CharacterChanged(const Character &character)
{
  if (m_verbose) { Helper().CoutNl(__func__); }

  consoleSelect(&m_screen_top);

  consoleClear();

  std::stringstream s;
  s
    << "Condition: " << character.GetCondition() << "/" << character.GetInitialCondition() << '\n'
    << "Skill: " << character.GetSkill() << "/" << character.GetInitialSkill() << '\n'
    << "Luck: " << character.GetLuck() << "/" << character.GetInitialLuck() << '\n'
    << "Gold: " << character.GetGold() << '\n'
    << "Provisions: " << character.GetProvisions() << '\n'
    << "Items:\n"
  ;
  {
    for (const auto item: character.GetItems())
    {
      if (static_cast<int>(item) < 100)
      {
        s << " * " << ToPrettyStr(item) << '\n';
      }
    }
  }
  Helper().CoutNl(s.str());
  consoleSelect(&m_screen_bottom);
}

Option NdsGameDialog::RequestOption(const std::vector<Option>& options)
{
  if (m_verbose) { Helper().CoutNl(__func__); }

  swiWaitForVBlank();

  m_options = options;
  std::vector<int> valid_indices;
  std::stringstream text;
  const int n_options{static_cast<int>(options.size())};
  for (int i=0; i!=n_options; ++i)
  {
    valid_indices.push_back(i);
    text << "[" << i << "] " << options[i].GetText() << '\n';
  }
  ShowText(text.str());
  if (n_options == 1) { return options[0]; }

  while(1)
  {
    ProcessEvents();

    if (m_key_pressed != -1)
    {
      int key_pressed = m_key_pressed;
      if (key_pressed >= 0 && key_pressed < static_cast<int>(options.size()))
      {
        m_key_pressed = -1;
        return options[key_pressed];
      }
      else
      {
        m_key_pressed = -1;
      }
    }
  }
}

void NdsGameDialog::ShowText(const std::string& text)
{
  if (m_verbose) { Helper().CoutNl(__func__); }

  consoleSelect(&m_screen_bottom);

  const int n_chars{22};
  const double wait_character_msec{0.01};
  int pos = 0;
  for (const char c: text)
  {
    if (c == '\n') pos = -1;
    else if (c == ' ' && pos > n_chars) { pos = 0; Helper().Cout(std::string(1,'\n')); continue; }
    Helper().Cout(std::string(1,c));
    ++pos;

    Helper().Wait(wait_character_msec);
  }
}

void NdsGameDialog::Start()
{
  if (m_verbose) { Helper().Cout(__func__); Helper().CoutNl(": 1"); }

  Dice::Get()->SetSeed(42);

  if (m_verbose) { Helper().CoutNl(__func__); Helper().CoutNl(": 2"); }

  MenuDialog menu;

  if (m_verbose) { Helper().CoutNl(__func__); Helper().CoutNl(": 3"); }

  menu.SetObserver(this);

  if (m_verbose) { Helper().CoutNl(__func__); Helper().CoutNl(": 4"); }

  menu.Execute();
}

void NdsGameDialog::ProcessEvents()
{
  if (m_verbose) { Helper().CoutNl(__func__); }

  scanKeys(); //Don't forget!
  const int keys_down = keysDown();
  if (keys_down)
  {
    if (     keys_down & KEY_UP   ) { m_key_pressed = 0; }
    else if (keys_down & KEY_RIGHT) { m_key_pressed = 1; }
    else if (keys_down & KEY_DOWN ) { m_key_pressed = 2; }
    else if (keys_down & KEY_LEFT ) { m_key_pressed = 3; }
    else if (keys_down & KEY_B    ) { m_key_pressed = 4; }
    else if (keys_down & KEY_A    ) { m_key_pressed = 5; }
  }

  swiWaitForVBlank();
}


void NdsGameDialog::Wait()
{
  if (m_verbose) { Helper().CoutNl(__func__); }

  swiWaitForVBlank();

  Helper().Wait(1.0);
}
