#include "helper.h"

#include <cassert>
#include <fstream>

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

void ShowText(const std::string& text, const bool auto_play)
{
  for (const char c: text)
  {
    std::cout << c;
    if (!auto_play)
    {
      std::cout.flush();
      Wait(0.01);
    }
  }
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
