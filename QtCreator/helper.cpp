#include "helper.h"

#include <cassert>
#include <fstream>
#include <iostream>

#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>

#include "ai.h"
#include "chapter.h"
#include "chaptertype.h"

void CreateGraph()
{
  const std::string filename_base{"Graph"};
  const std::string dot_filename{filename_base + ".dot"};
  const std::string png_filename{filename_base + ".png"};
  std::ofstream f(dot_filename);

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
  {
    std::stringstream cmd;
    cmd << "dot -Tpng " << dot_filename << " > " << png_filename;
    const int error{std::system(cmd.str().c_str())};
    assert(!error);
    if (error) {;}
  }
}

std::string FileToString(const std::string& filename)
{
  if (!IsRegularFile(filename))
  {
    std::stringstream msg;
    msg << __func__ << ": ERROR: File " << filename << " does not exist";
    throw std::runtime_error(msg.str());
  }
  const std::vector<std::string> lines = FileToVector(filename);
  std::stringstream s;
  std::copy(std::begin(lines),std::end(lines),std::ostream_iterator<std::string>(s," "));
  return s.str();
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

  boost::algorithm::trim(text); //Remove the space between text and @
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
