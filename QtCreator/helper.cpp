#include "helper.h"

#include <cassert>
#include <fstream>
#include <iostream>

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


bool IsRegularFile(const std::string& filename)
{
  std::fstream f;
  f.open(filename.c_str(),std::ios::in);
  return f.is_open();
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

void Wait(const double n_secs) noexcept
{
  boost::timer t;
  while (t.elapsed() < n_secs) {}
}
