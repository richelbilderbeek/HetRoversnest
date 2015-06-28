#include "dochapter.h"

#include <algorithm>
#include <cassert>
#include <string>
#include <iterator>
#include <iostream>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include "character.h"
#include "helper.h"

void DoChapter(
  int& chapter,
  Character& character,
  const bool auto_play
)
{
  assert(character.GetCondition() > 0);

  const std::string filename{"../Bestanden/" + std::to_string(chapter) + ".txt"};
  if (!IsRegularFile(filename))
  {
    std::stringstream msg;
    msg << __func__ << ": ERROR: File " << filename << " does not exist";
    throw std::runtime_error(msg.str());
  }
  const std::vector<std::string> lines = FileToVector(filename);
  std::stringstream s;
  s << std::noskipws;
  std::copy(std::begin(lines),std::end(lines),std::ostream_iterator<std::string>(s," "));

  //Show text until @
  {
    int pos = 0;
    char prev_c = ' ';
    while (s)
    {
      char c;
      s >> c;
      if (c == '@') break; //Now the options must be parsed
      if (c == '\n') c = ' '; //Convert a newline to space, so input files do not require a space after every line
      if (c == ' ' && pos == 0) continue; //Een nieuwe regel begint niet met een spatie
      if (c == ' ' && prev_c == ' ') continue; //Tweede spatie overslaan
      std::cout << c;
      prev_c = c;
      ++pos;
      if (pos > 60 && c == ' ')
      {
        std::cout << '\n';
        pos = 0;
      }
    }
  }
  std::cout << std::endl;
  char chapter_type = '?';
  s >> chapter_type;
  switch (chapter_type)
  {
    case '0': DoNormalChapter(s,chapter,auto_play); break;
    default:
    {
      std::stringstream msg;
      msg << __func__ << ": ERROR: Chapter " << chapter << " does not have a (supported) code, "
        << "chapter_type: " << chapter_type
        << std::endl; return;
      throw std::runtime_error(msg.str());
    }
  }
}


void DoNormalChapter(std::stringstream& s, int& chapter, const bool auto_play)
{
  //Parse the options
  std::vector<std::pair<char,int>> options; //input, new chapter
  {
    while (!s.eof())
    {
      char at;
      assert(!s.eof());
      s >> at;
      assert(!s.eof());
      while (!s.eof() && (at == '\n' || at == ' ')) { s >> at; }
      if(s.eof()) break;
      assert(at == '@');
      assert(!s.eof());
      char option;
      assert(!s.eof());
      s >> option;
      char colon;
      assert(!s.eof());
      s >> colon;
      assert(colon == ':');
      int chapter_to_go;
      assert(!s.eof());
      s >> chapter_to_go;
      options.push_back(std::make_pair(option,chapter_to_go));
    }
  }
  assert(!options.empty());
  //Chose an options
  if (auto_play)
  {
    std::random_shuffle(std::begin(options),std::end(options));
    std::cout << "AUTOPLAY: chose option " << options[0].first << std::endl;
    chapter = options[0].second;
    return;
  }
  //Process command
  while (1)
  {
    std::string s;
    std::getline(std::cin,s);
    assert(!s.empty());
    const char command = s[0];
    //std::cerr << "command: " << command << std::endl;
    const auto iter = std::find_if(std::begin(options),std::end(options),
      [command](const std::pair<char,int> p) { return p.first == command; }
    );
    if (iter == std::end(options))
    {
      std::stringstream s;
      s << "Deze optie bestaat niet. Geldige opties zijn ";
      std::transform(std::begin(options),std::end(options),std::ostream_iterator<std::string>(s,", "),
        [](const std::pair<char,int> p) { return boost::lexical_cast<std::string>(p.first); }
      );

      std::string t{s.str()};
      t.pop_back();
      t.pop_back();
      std::cout << t << std::endl;
      continue;
    }
    //Success!
    //std::cerr << "Success." << std::endl;
    chapter = iter->second;
    break;
  }
}
