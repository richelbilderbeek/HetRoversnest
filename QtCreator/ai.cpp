#include "ai.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>

#include "chapter.h"
#include "dice.h"
#include "game.h"
#include "helper.h"

Ai::Ai()
  :
    m_distribution{std::uniform_real_distribution<double>(0.0,1.0)},
    m_engine(0),
    m_game{nullptr},
    m_keys{},
    m_payoffs{}
{
  #ifndef NDEBUG
  Test();
  #endif

  //Collect all hashes of all options
  /*
  for (int i=1; i!=450; ++i)
  {
    try
    {
      Chapter chapter(i);
      for (const auto option: chapter.GetOptions().GetOptions())
      {
        const std::string text{option.GetText()};
        m_payoffs.push_back(PayoffPair(text,1.0));
      }
    }
    catch (std::runtime_error& e)
    {
      std::cout << e.what() << std::endl;
    }
  }

  m_payoffs.push_back(PayoffPair(CreateShowInventoryOption().GetText(),0.0));
  m_payoffs.push_back(PayoffPair(CreateYesOption().GetText(),0.0));
  m_payoffs.push_back(PayoffPair(CreateNoOption().GetText(),1.0));
  m_payoffs.push_back(PayoffPair(CreateLeaveOption().GetText(),1.0));

  //Check
  //std::cout << (*this) << std::endl;
  */
}


void Ai::CreateGraph() const noexcept
{
  const std::string filename_base{"Payoffs"};
  const std::string dot_filename{filename_base + ".dot"};
  const std::string png_filename{filename_base + ".png"};
  std::ofstream f(dot_filename);

  f << "digraph CityOfThievesPayoffs {\n";
  for (int i=1; i!=450; ++i)
  {
    try
    {
      const Chapter chapter(i);
      //Label node according to chapter type
      std::string node_color = "black";
      switch (chapter.GetType())
      {
        case ChapterType::fight: node_color = "red"; break;
        case ChapterType::test_your_luck: node_color = "blue"; break;
        case ChapterType::test_your_skill: node_color = "green"; break;
        default: break; //OK
      }
      f << i
        << "["
        << "label =\""
        << std::to_string(chapter.GetChapterNumber())
        << "\"];\n"
      ;

      if (chapter.GetNextChapter() != -1)
      {
        f << i << "->" << chapter.GetNextChapter() << ";\n";
      }
      else if (!chapter.GetFighting().GetMonsters().empty())
      {

        if (chapter.GetFighting().GetEscapeToChapter() != -1)
        {
          f << i << "->" << chapter.GetFighting().GetEscapeToChapter() << " [ label = \"Escape\"];\n";
        }
      }
      else if (!chapter.GetLuck().GetLuckText().empty())
      {
        f << i << "->" << chapter.GetLuck().GetLuckChapter() << " [ label = \"Luck\"];\n";
        f << i << "->" << chapter.GetLuck().GetNoLuckChapter() << " [ label = \"No luck\"];\n";
      }
      else if (!chapter.GetOptions().GetOptions().empty())
      {
        for (const auto option: chapter.GetOptions().GetOptions())
        {
          //No payoff
          //f << i << "->" << option.GetNextChapter() << " [ label = \"Choice\"];\n";
          f << i << "->" << option.GetNextChapter() << " [ label = \""
            << GetPayoff(option.GetText()) <<  "\"];\n";
        }
      }
      else if (!chapter.GetSkill().GetSkillText().empty())
      {
        f << i << "->" << chapter.GetSkill().GetSkillConsequence().GetNextChapter() << " [ label = \"Skill\"];\n";
        f << i << "->" << chapter.GetSkill().GetNoSkillConsequence().GetNextChapter() << " [ label = \"No skill\"];\n";
      }
      else if (chapter.GetType() == ChapterType::game_lost)
      {
        f << i << "-> GameOver;\n";
      }
      else if (chapter.GetType() == ChapterType::game_won)
      {
        f << i << "-> GameWon;\n";
      }
    }
    catch (std::logic_error& e)
    {
      //f << i << ": FAIL" << std::endl;
    }
    catch (std::runtime_error& e)
    {
      //f << i << ": not present" << std::endl;
    }
  }
  f << "}\n";
  f.close();
  {
    std::stringstream cmd;
    cmd << "dot -Tpng " << dot_filename << " > " << png_filename;
    const int error{std::system(cmd.str().c_str())};
    assert(!error);
    if (error) {;}
  }
}

double Ai::GetPayoff(const std::string& option_text) const noexcept
{
  const auto iter
   = std::find_if(
       std::begin(m_payoffs),
       std::end(m_payoffs),
       [option_text](const PayoffPair& pair)
       {
         return pair.first == option_text;
       }
     );
  if (iter == std::end(m_payoffs))
  {
    const_cast<Ai*>(this)->m_payoffs.push_back(PayoffPair(option_text,1.0));
    return GetPayoff(option_text);
  }
  assert(iter != std::end(m_payoffs));
  return iter->second;
}


void Ai::SetFinalPayoff(const Payoff& final_payoff)
{
  assert(!m_keys.empty());
  double weight{1.0};
  for (int i=0; ; ++i)
  {
    weight *= 0.9;
    assert(!m_keys.empty());
    const auto key = m_keys.back();
    const double current_key_payoff{GetPayoff(key)};
    const double new_chapter_payoff{
      current_key_payoff + (weight * (final_payoff - current_key_payoff))
    };
    if (!IsBetween(new_chapter_payoff,current_key_payoff,final_payoff))
    {
      std::cerr
        << "Now: " << current_key_payoff << '\n'
        << "Final: " << final_payoff << '\n'
        << "Weight: " << weight<< '\n'
        << "New: " << new_chapter_payoff << '\n'
      ;
    }
    assert(IsBetween(new_chapter_payoff,current_key_payoff,final_payoff));
    SetPayoff(key,new_chapter_payoff);
    m_keys.pop_back();
    if (m_keys.empty()) break;
  }
  m_keys.clear();
}

void Ai::SetPayoff(const Key& key, const Payoff& payoff)
{
  assert(payoff >= 0.0);

  auto iter = std::find_if(
    std::begin(m_payoffs),
    std::end(m_payoffs),
    [key](const PayoffPair& pair)
    {
      return pair.first == key;
    }
  );
  assert(iter != std::end(m_payoffs));
  iter->second = payoff;
}

void Ai::Start()
{
  std::cout << "SOLVING THE GAME" << std::endl;

  for (int i=1; ; ++i)
  {
    //if i equals 1,10,100,1000,etc...
    //if (i % 10000 == 0)
    if (static_cast<int>(std::log10(i)) != static_cast<int>(std::log10(i-1)))
    {
      std::cout << "i: " << i << std::endl;
      //this->CreateGraph();
      std::cout << "DONE\n";
    }

    const Character character(6+6,12+6,6+6,Item::luck_potion);
    Game game(42,character);
    m_game = &game;

    game.m_signal_request_option.connect(
      boost::bind(&Ai::SlotRequestOption,this,_1)
    );
    game.m_signal_wait.connect(
      boost::bind(&Ai::SlotWait,this)
    );
    game.m_signal_show_text.connect(
      boost::bind(&Ai::SlotShowText,this,_1)
    );

    while (1)
    {
      game.DoChapter();
      if (game.HasLost() || game.HasWon()) break;
    }

    const double final_payoff{
        character.HasItem(Item::black_pearls) ? 2.0 : 0.0
      + character.HasItem(Item::lotus_flower) ? 2.0 : 0.0
      + character.HasItem(Item::hags_hair) ? 2.0 : 0.0
      + character.HasItem(Item::tattoo) ? 2.0 : 0.0
      + character.HasItem(Item::silver_arrow) ? 2.0 : 0.0
      + game.HasWon() ? 2.0 : 0.0
    };
    SetFinalPayoff(final_payoff);
    if (game.HasWon()) { break; }
  }
  std::cout << "FINISHED THE GAME, CREATING GRAPH" << std::endl;
  this->CreateGraph();
  assert(1==2);
}

#ifndef NDEBUG
void Ai::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
}
#endif

std::ostream& operator<<(std::ostream& os, const Ai& ai)
{
  const auto payoffs = ai.GetPayoffs();
  for (const auto payoff: payoffs)
  {
    os << "(" << payoff.first << "," << payoff.second << ")\n";
  }
  return os;
}

Option Ai::SlotRequestOption(const std::vector<Option>& options)
{
  for (const auto option: options)
  {
    SlotShowText(" * " + option.GetText() + "\n");
  }
  assert(!options.empty());
  if (options.size() == 1) return options[0];
  if (options[0].GetConsequence().GetType() == ConsequenceType::no)
  {
    return options[0]; //Always say no
  }

  std::vector<double> expected_payoffs;
  std::transform(
    std::begin(options),
    std::end(options),
    std::back_inserter(expected_payoffs),
    [this](const Option& option)
    {
      const auto text = option.GetText();
      const double payoff{GetPayoff(text)};
      assert(payoff >= 0.0);
      return payoff;
    }
  );

  const int n_options{static_cast<int>(options.size())};

  const bool choose_probabilisticly{true};
  if (choose_probabilisticly)
  {
    const double sum{std::accumulate(std::begin(expected_payoffs),std::end(expected_payoffs),0.0)};
    if (sum != 0.0)
    {
      double f{m_distribution(m_engine) * sum};
      for (int i=0; i!=n_options; ++i)
      {
        const auto text = options[i].GetText();
        if (f < GetPayoff(text))
        {
          m_keys.push_back(text);
          return options[i];
        }
        f -= GetPayoff(text);
      }
      //Should not get here often
    }
    //Choose first option
    m_keys.push_back(options[0].GetText());
    return options[0];
  }
  //Choose best
  int best_option{0};
  double expected_payoff{GetPayoff(options[0].GetText())};
  for (int i=1; i!=n_options; ++i)
  {
    double this_expected_payoff{GetPayoff(options[i].GetText())};
    if (this_expected_payoff > expected_payoff)
    {
      best_option = i;
      expected_payoff = this_expected_payoff;
    }
  }
  m_keys.push_back(options[best_option].GetText());
  return options[best_option];
}

void Ai::SlotShowText(const std::string& text)
{
  const int n_chars{60};
  int pos = 0;
  for (const char c: text)
  {
    if (c == '\n') pos = -1;
    else if (c == ' ' && pos > n_chars) { pos = 0; std::cout << '\n'; continue; }
    else if (c == ' ' && pos == 0) { continue; }
    std::cout << c;
    ++pos;
    std::cout.flush();
  }
}

void Ai::SlotWait()
{
  //Continue
}
