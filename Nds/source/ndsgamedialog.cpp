#include "ndsgamedialog.h"

#include <cassert>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <nds.h>
#include <filesystem.h>

#include "chapter.h"
#include "menudialog.h"
#include "helper.h"
#include "dice.h"


NdsGameDialog::NdsGameDialog()
  : m_key_pressed{-1},
    m_options{},
    m_screen_bottom{},
    m_screen_top{}
{
  nitroFSInit(nullptr);

  videoSetMode(MODE_0_2D);
  videoSetModeSub(MODE_0_2D);

  vramSetBankA(VRAM_A_MAIN_BG);
  vramSetBankC(VRAM_C_SUB_BG);

  consoleInit(&m_screen_top,    3,BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
  consoleInit(&m_screen_bottom, 3,BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);

  consoleSelect(&m_screen_top);

  const Helper h;

  try
  {
    std::cout << h.FileToString("1.txt") << std::endl;
  }
  catch (std::runtime_error& e)
  {
    std::cout << e.what() << std::endl;
  }
  consoleClear();

  consoleSelect(&m_screen_bottom);

  try
  {
    std::cout << h.FileToString("2.txt") << std::endl;
  }
  catch (std::runtime_error& e)
  {
    std::cout << e.what() << std::endl;
  }
  consoleClear();

}


void NdsGameDialog::CharacterChanged(const Character &character)
{
  if (m_verbose) { std::cout << __func__ << std::endl; }

  consoleSelect(&m_screen_top);

  consoleClear();

  std::cout
    << "Condition: " << character.GetCondition() << "/" << character.GetInitialCondition()
    << std::endl
  ;

  std::cout
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
        std::cout << ToPrettyStr(item) << std::endl;
      }
    }
  }

  consoleSelect(&m_screen_bottom);
}

Option NdsGameDialog::RequestOption(const std::vector<Option>& options)
{
  if (m_verbose) { std::cout << __func__ << std::endl; }

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
    swiWaitForVBlank();

    if (m_key_pressed != -1)
    {
      int key_pressed = m_key_pressed;
      assert(key_pressed >= 0);
      assert(key_pressed < static_cast<int>(options.size()));
      m_key_pressed = -1;
      return options[key_pressed];
    }
  }
}

void NdsGameDialog::ShowText(const std::string& text)
{
  if (m_verbose) { std::cout << __func__ << std::endl; }

  consoleSelect(&m_screen_bottom);

  const int n_chars{22};
  const double wait_character_msec{0.01};
  int pos = 0;
  for (const char c: text)
  {
    swiWaitForVBlank();

    if (c == '\n') pos = -1;
    else if (c == ' ' && pos > n_chars) { pos = 0; std::cout << '\n'; continue; }
    std::cout << c;
    ++pos;
    std::cout.flush();

    swiWaitForVBlank();

    Helper().Wait(wait_character_msec);
  }
}

void NdsGameDialog::Start()
{
  if (m_verbose) { std::clog << __func__ << "1" << std::endl; }

  Dice::Get()->SetSeed(42);

  if (m_verbose) { std::clog << __func__ << "2" <<std::endl; }

  MenuDialog menu;

  if (m_verbose) { std::clog << __func__ << "3" <<std::endl; }

  menu.SetObserver(this);

  if (m_verbose) { std::clog << __func__ << "4" <<std::endl; }

  menu.Execute();
}

void NdsGameDialog::ProcessEvents()
{
  if (m_verbose) { std::cout << __func__ << std::endl; }

  swiWaitForVBlank();

  m_key_pressed = 0;
}


void NdsGameDialog::Wait()
{
  if (m_verbose) { std::cout << __func__ << std::endl; }

  swiWaitForVBlank();

  Helper().Wait(1.0);
}
