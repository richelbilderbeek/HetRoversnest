#include "helper.h"

#include <cassert>
#include <fstream>
#include <iostream>

#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>

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
  char c;
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
      break; //Now the options must be parsed
    }
    if (c == '\n') c = ' '; //Convert a newline to space, so input files do not require a space after every line
    //if (c == ' ' && pos == 0) continue; //Een nieuwe regel begint niet met een spatie
    if (c == ' ' && prev_c == ' ') continue; //Tweede spatie overslaan
    //if (c == '[') { text << '\n'; pos = 0; }
    text += c;
    prev_c = c;
    //++pos;
    //if (pos > line_length && c == ' ')
    //{
    //  text += '\n';
    //  pos = 0;
    //}
  }
  s << std::skipws; //Obligatory
  return text;
}

void ShowText(const std::string& text, const bool auto_play)
{
  int pos = 0;
  for (const char c: text)
  {
    //if (c == '[') std::cout << '\n';
    if (c == '\n') pos = -1;
    if (c == ' ' && pos > 60) { pos = 0; std::cout << '\n'; continue; }
    std::cout << c;
    ++pos;
    if (!auto_play)
    {
      std::cout.flush();
      #ifndef NDEBUG
      Wait(0.001);
      #else
      Wait(0.01);
      #endif
    }
  }
}

void SpeakText(const std::string& text, const bool auto_play)
{
  if (auto_play) return;
  std::ofstream f("espeak.txt");
  f << text;
  f.close();
  const int error{std::system("espeak -f espeak.txt")};
  assert(!error);
  if (error) {;}
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
