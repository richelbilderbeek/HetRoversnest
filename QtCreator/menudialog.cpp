#include "menudialog.h"

#include <cassert>
#include <string>
#include <ctime>
#include <sstream>
#include <fstream>
#include <iterator>
#include <memory>

#include "ai.h"
#include "dice.h"
#include "terminal.h"
#include "helper.h"
#include "game.h"
#include "walkthrough.h"

MenuDialog::MenuDialog()
  : m_observer{nullptr}
{
  #ifndef NDEBUG
  Test();
  #endif
}

Character MenuDialog::CreateCharacter() const noexcept
{
  ShowText("Character generation\n");
  ShowText("\n");

  //Create stats
  int skill = -1;
  int condition = -1;
  int luck = -1;
  while (1)
  {
    //In the original version, skill equals 6 plus a dice roll
    //In this version, however, 3 skill points are gained by the shield and Carralif's sword
    skill = 3 + Dice::Get()->Throw();
    condition = Dice::Get()->Throw() + Dice::Get()->Throw();
    luck = 6 + Dice::Get()->Throw();
    const int min{3 + 1 + 1 + 1 + 6 + 1};
    const int max{3 + 6 + 6 + 6 + 6 + 6};
    const int sum{skill + condition + luck};
    const double f{ static_cast<double>(sum - min) / static_cast<double>(max - min) };
    const int difficulty{static_cast<int>((1.0 - f) * 100.0)};
    std::stringstream text;
    text
      << "Skill: " << skill << '\n'
      << "Condition: " << condition << '\n'
      << "Luck: " << luck << '\n'
      << "Difficulty: " << difficulty << "% \n"
      << "\n"
      << "Keep this character?\n"
      << "\n"
    ;
    ShowText(text.str().c_str());
    const auto chosen = RequestOption(CreateYesNoOptions());
    assert(chosen.GetText() == "Yes" || chosen.GetText() == "No");
    if (chosen.GetText() == "Yes") break;
  }
  ShowText("\n");
  Item initial_potion = Item::shield;
  {
    ShowText("Choose the potion you take with you\n");
    ShowText("\n");
    const auto chosen = RequestOption(CreateInitialPotionOptions());
    assert(chosen.GetConsequence().GetItemsToAdd().size() == 1);
    initial_potion = chosen.GetConsequence().GetItemsToAdd()[0];
  }
  const Character character(skill,condition,luck,initial_potion);
  return character;
}

void MenuDialog::Execute()
{
  ShowText("City Of Thieves\n");
  while (1)
  {
    ShowText("\n");
    const auto chosen = RequestOption(GetMenuOptions());
    ShowText("\n");
    if (chosen.GetText() == GetStartGameText()) { StartGame(); }
    else if (chosen.GetText() == GetTeaserText()) { ShowTeaser(); }
    else if (chosen.GetText() == GetIntroductionText()) { ShowIntroduction(); }
    else if (chosen.GetText() == GetManualText()) { ShowManual(); }
    else if (chosen.GetText() == GetHintsText()) { ShowHints(); }
    else if (chosen.GetText() == GetAboutText()) { ShowAbout(); }
    else if (chosen.GetText() == GetCreateGraphText()) { CreateGraph(); }
    else if (chosen.GetText() == GetSolveGameText()) { SolveGame(); }
    else if (chosen.GetText() == GetQuitText()) { return; }
    ShowText("\n");
  }
}

std::vector<Option> MenuDialog::GetMenuOptions() const noexcept
{
  std::vector<Option> options;
  options.push_back(Option(GetStartGameText(),Consequence()));
  options.push_back(Option(GetTeaserText(),Consequence()));
  options.push_back(Option(GetIntroductionText(),Consequence()));
  options.push_back(Option(GetManualText(),Consequence()));
  options.push_back(Option(GetHintsText(),Consequence()));
  options.push_back(Option(GetAboutText(),Consequence()));
  #ifndef NDEBUG
  options.push_back(Option(GetCreateGraphText(),Consequence()));
  options.push_back(Option(GetSolveGameText(),Consequence()));
  #endif
  options.push_back(Option(GetQuitText(),Consequence()));
  return options;
}

void MenuDialog::ShowAbout()
{
  {
    const std::string filename{"../Files/About.txt"};
    const std::string text{FileToString(filename)};
    ShowText(text);
    //
  }
  ShowText("\n");
  {
    const std::string filename{"../Files/Changelog.txt"};
    const std::string text{FileToString(filename)};
    ShowText(text);

  }
}

void MenuDialog::ShowIntroduction()
{
  const std::string filename{"../Files/Introduction.txt"};
  const std::string text{FileToString(filename)};
  ShowText(text);

}

void MenuDialog::ShowManual()
{
  const std::string filename{"../Files/Manual.txt"};
  const std::string text{FileToString(filename)};
  ShowText(text);

}

void MenuDialog::ShowTeaser()
{
  const std::string filename{"../Files/Teaser.txt"};
  const std::string text{FileToString(filename)};
  ShowText(text);

}

void MenuDialog::ShowHints()
{
  const std::string filename{"../Files/Hints.txt"};
  const std::string text{FileToString(filename)};
  ShowText(text);

}

void MenuDialog::StartGame()
{
  #ifndef NDEBUG
  const int seed{49};
  #else
  std::random_device rd;
  const int seed{static_cast<int>(rd())};
  #endif

  const Character character = CreateCharacter();

  Game game(seed,character);
  game.SetObserver(m_observer);

  m_observer->CharacterChanged(game.GetCharacter()); //Show initial status

  while (1)
  {
    game.DoChapter();
    if (game.HasWon() || game.HasLost()) break;
  }
}

Option MenuDialog::RequestOption(const std::vector<Option>& options) const
{
  assert(m_observer);
  return m_observer->RequestOption(options);
}

void MenuDialog::ShowText(const std::string& text) const
{
  assert(m_observer);
  m_observer->ShowText(text);
}

void MenuDialog::CharacterChanged(const Character& character) const
{
  assert(m_observer);
  m_observer->CharacterChanged(character);
}

void MenuDialog::Wait() const
{
  assert(m_observer);
  m_observer->Wait();
}

#ifndef NDEBUG
void MenuDialog::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  TestHelperFunctions();
  const Character character(6+6,12+6,6+6,Item::luck_potion);
  const int seed{42};
  Game game(seed,character);

  Terminal dialog;

  MenuDialog menu;
  //menu.Execute();

}
#endif
