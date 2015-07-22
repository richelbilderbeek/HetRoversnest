#include "helper.h"

#include <cassert>
#include <fstream>
#include <iostream>

#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>

#include "ai.h"
#include "chapter.h"
#include "chaptertype.h"

void CreateGraph(const Ai * const ai)
{
  std::clog << "Creating dot file..." << std::endl;
  const std::string filename{ai ? "Payoffs.dot" : "Graph.dot"};
  std::ofstream f(filename);

  f << "digraph CityOfThieves {\n";
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
      ;
      if (ai) f << " (" << ai->GetPayoff(i) <<  ")";
      f
        << "\""
        << ",color=\"" << node_color << "\""
      ;
      if (ai)
      {
        std::string fill_color{"#ffffff"};
        const double payoff{ai->GetPayoff(i)};
        if      (payoff < 0.00000001 ) { fill_color = "#ffffff"; }
        else if (payoff < 0.0000001) { fill_color = "#444444"; }
        else if (payoff < 0.000001) { fill_color = "#555555"; }
        else if (payoff < 0.00001) { fill_color = "#666666"; }
        else if (payoff < 0.0001) { fill_color = "#777777"; }
        else if (payoff < 0.001) { fill_color = "#888888"; }
        else if (payoff < 0.01) { fill_color = "#997777"; }
        else if (payoff < 0.1) { fill_color = "#aaaa88"; }
        else if (payoff <  1) { fill_color = "#99bb99"; }
        else if (payoff <  2) { fill_color = "#aacccc"; }
        else if (payoff <  4) { fill_color = "#bbbbdd"; }
        else if (payoff <  8) { fill_color = "#eeccee"; }
        else if (payoff < 16) { fill_color = "#ffffff"; }
        f << ",style=filled,fillcolor=\"" << fill_color << "\"";
      }
      f
        << "];\n"
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
          f << i << "->" << option.GetNextChapter() << " [ label = \"Choice\"];\n";
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
  std::clog << "Creating graph..." << std::endl;
  if (!ai)
  {
    const int error{std::system("dot -Tpng Graph.dot > Graph.png")};
    assert(!error);
    if (error) {;}
  }
  else
  {
    const int error{std::system("dot -Tpng Payoffs.dot > Payoffs.png")};
    assert(!error);
    if (error) {;}
  }
  std::clog << "Graph created" << std::endl;
}

std::vector<std::string> FileToVector(const std::string& filename)
{
  assert(IsRegularFile(filename));
  std::vector<std::string> v;
  std::ifstream in(filename.c_str());
  for (int i=0; !in.eof(); ++i)
  {
    std::string s;
    std::getline(in,s);
    v.push_back(s);
  }
  return v;
}

bool IsBetween(const double x, const double a, const double b)
{
  // a < x < b
  if (a <= x && x <= b) return true;
  if (b <= x && x <= a) return true;
  return false;
}

bool IsInt(const std::string& s) noexcept
{
  try
  {
    boost::lexical_cast<int>(s);
    return true;
  }
  catch (boost::bad_lexical_cast&)
  {
    return false;
  }
}


bool IsRegularFile(const std::string& filename)
{
  std::fstream f;
  f.open(filename.c_str(),std::ios::in);
  return f.is_open();
}

void Parse(std::stringstream& s, const char expected_char)
{
  const char c = ReadChar(s);
  if (c != expected_char) { std::cerr << "WARNING: expected character" << std::endl; }
  assert(c == expected_char);
}

char ReadChar(std::stringstream& s)
{
  char c = '\0';
  assert(!s.eof());
  s >> c;
  assert(!s.eof());
  while (!s.eof() && (c == '\n' || c == ' ' || c == '\0')) { s >> c; }
  assert(!s.eof());
  return c;
}


int ReadInt(std::stringstream& s)
{
  int number = -9999;
  s >> number;
  assert(number > -9999);
  return number;
}

std::string ReadString(std::stringstream& s)
{
  std::string str = "";
  s >> str;
  return str;
}

std::string ReadText(std::stringstream& s)
{
  s << std::noskipws; //Obligatory
  //Show text until @
  std::string text;
  //int pos = 0;
  char prev_c = ' ';
  while (1)
  {
    char c;
    s >> c;
    if (c == '@')
    {
      break;
    }
    if (c == '\n') c = ' '; //Convert a newline to space, so input files do not require a space after every line
    if (c == ' ' && prev_c == ' ') continue; //Tweede spatie overslaan
    text += c;
    prev_c = c;
  }
  s << std::skipws; //Obligatory
  return text;
}


std::vector<std::string> StripFirstChar(std::vector<std::string> v)
{
  assert(!v.empty());
  assert(!v[0].empty());
  v[0] = v[0].substr(1,v[0].size() - 1);
  return v;
}


std::string ToPretty(std::string s)
{
  assert(!s.empty());
  if (s == "carralifs_sword") { return "Carralif's sword"; }
  std::replace(
    std::begin(s),
    std::end(s),
    '_',' '
  );
  s[0] = std::tolower(s[0]);
  return s;
}

void Wait(const double n_secs) noexcept
{
  boost::timer t;
  while (t.elapsed() < n_secs) {}
}
