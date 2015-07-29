#include "menudialog.h"

#include <string>
#include <ctime>
#include <sstream>
#include <fstream>
#include <iterator>
#include <memory>

#include <QFile>

#include "ai.h"
#include "dice.h"
#include "terminal.h"
#include "helper.h"
#include "game.h"
#include "walkthrough.h"

MenuDialog::MenuDialog()
  :
    m_signal_character_has_changed{},
    m_signal_request_option{},
    m_signal_show_text{},
    m_signal_wait{}
{
  #ifndef NDEBUG
  Test();
  #endif
}

Character MenuDialog::CreateCharacter() const noexcept
{
  m_signal_show_text("Character generation\n");
  m_signal_show_text("\n");

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
    m_signal_show_text(text.str().c_str());
    const auto chosen = *m_signal_request_option(CreateYesNoOptions());
    assert(chosen.GetText() == "Yes" || chosen.GetText() == "No");
    if (chosen.GetText() == "Yes") break;
  }
  m_signal_show_text("\n");
  Item initial_potion = Item::shield;
  {
    m_signal_show_text("Choose the potion you take with you\n");
    m_signal_show_text("\n");
    const auto chosen = *m_signal_request_option(CreateInitialPotionOptions());
    assert(chosen.GetConsequence().GetItemsToAdd().size() == 1);
    initial_potion = chosen.GetConsequence().GetItemsToAdd()[0];
  }
  const Character character(skill,condition,luck,initial_potion);
  return character;
}

void MenuDialog::Execute()
{
  m_signal_show_text("City Of Thieves\n");
  while (1)
  {
    m_signal_show_text("\n");
    const auto chosen = m_signal_request_option(GetMenuOptions());
    m_signal_show_text("\n");
    if (chosen->GetText() == GetStartGameText()) { StartGame(); }
    else if (chosen->GetText() == GetTeaserText()) { ShowTeaser(); }
    else if (chosen->GetText() == GetIntroductionText()) { ShowIntroduction(); }
    else if (chosen->GetText() == GetManualText()) { ShowManual(); }
    else if (chosen->GetText() == GetHintsText()) { ShowHints(); }
    else if (chosen->GetText() == GetAboutText()) { ShowAbout(); }
    else if (chosen->GetText() == GetCreateGraphText()) { CreateGraph(); }
    else if (chosen->GetText() == GetSolveGameText()) { SolveGame(); }
    else if (chosen->GetText() == GetQuitText()) { return; }
    m_signal_show_text("\n");
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
    const std::string filename{"About.txt"};
    {
      QFile qfile( (":/files/" + filename).c_str() );
      qfile.copy( filename.c_str() );
    }
    const std::string text{FileToString(filename)};
    m_signal_show_text(text);
    std::remove(filename.c_str());
  }
  m_signal_show_text("\n");
  {
    const std::string filename{"Changelog.txt"};
    {
      QFile qfile( (":/files/" + filename).c_str() );
      qfile.copy( filename.c_str() );
    }
    const std::string text{FileToString(filename)};
    m_signal_show_text(text);
    std::remove(filename.c_str());
  }
}

void MenuDialog::ShowIntroduction()
{
  const std::string filename{"Introduction.txt"};
  {
    QFile qfile( (":/files/" + filename).c_str() );
    qfile.copy( filename.c_str() );
  }
  const std::string text{FileToString(filename)};
  m_signal_show_text(text);
  std::remove(filename.c_str());
}

void MenuDialog::ShowManual()
{
  const std::string filename{"Manual.txt"};
  {
    QFile qfile( (":/files/" + filename).c_str() );
    qfile.copy( filename.c_str() );
  }
  const std::string text{FileToString(filename)};
  m_signal_show_text(text);
  std::remove(filename.c_str());
}

void MenuDialog::ShowTeaser()
{
  const std::string filename{"Teaser.txt"};
  {
    QFile qfile( (":/files/" + filename).c_str() );
    qfile.copy( filename.c_str() );
  }
  const std::string text{FileToString(filename)};
  m_signal_show_text(text);
  std::remove(filename.c_str());
}

void MenuDialog::ShowHints()
{
  const std::string filename{"Hints.txt"};
  {
    QFile qfile( (":/files/" + filename).c_str() );
    qfile.copy( filename.c_str() );
  }
  const std::string text{FileToString(filename)};
  m_signal_show_text(text);
  std::remove(filename.c_str());
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

  game.m_signal_character_has_changed.connect(
    boost::bind(&MenuDialog::SlotCharacterChanged,this,_1)
  );
  game.m_signal_request_option.connect(
    boost::bind(&MenuDialog::SlotRequestOption,this,_1)
  );
  game.m_signal_wait.connect(
    boost::bind(&MenuDialog::SlotWait,this)
  );
  game.m_signal_show_text.connect(
    boost::bind(&MenuDialog::SlotShowText,this,_1)
  );

  m_signal_character_has_changed(game.GetCharacter()); //Show initial status

  while (1)
  {
    game.DoChapter();
    if (game.HasWon() || game.HasLost()) break;
  }
}

void MenuDialog::SlotCharacterChanged(const Character& character)
{
  m_signal_character_has_changed(character);
}

Option MenuDialog::SlotRequestOption(const std::vector<Option>& valid_options)
{
  return *m_signal_request_option(valid_options);
}

void MenuDialog::SlotShowText(const std::string& text)
{
  m_signal_show_text(text);
}

void MenuDialog::SlotWait()
{
  m_signal_wait();
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
