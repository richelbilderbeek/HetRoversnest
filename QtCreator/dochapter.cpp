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
    case '1': DoTestYourLuckChapter(s,chapter,character); break;
    case '2': DoTestYourDexterityChapter(s,chapter,character); break;
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
      if (chapter == chapter_to_go)
      {
        std::stringstream msg;
        msg << __func__ << ": ERROR: Chapter " << chapter << "\'s option " << option
          << " brings player to the same chapter";
        throw std::runtime_error(msg.str());
      }
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

void DoTestYourDexterityChapter(std::stringstream& s, int& chapter, Character& character)
{
  //Parse no dexterity
  int no_dex_chapter = -1;
  std::string no_dex_text;
  {
    char at;
    assert(!s.eof());
    s >> at;
    assert(!s.eof());
    while (!s.eof() && (at == '\n' || at == ' ')) { s >> at; }
    assert(!s.eof());
    assert(at == '@');
    char zero;
    s >> zero;
    while (!s.eof() && (zero == '\n' || zero == ' '))
    {
      s >> zero;
    }
    assert(zero == '0');
    char colon;
    s >> colon;
    assert(colon == ':');
    while (!s.eof())
    {
      char c;
      s >> c;
      if (c == '@') break;
      no_dex_text += c;
    }
    s >> no_dex_chapter;
    assert(no_dex_chapter > -1);
    assert(!no_dex_text.empty());
  }

  //Parse dex
  int dex_chapter = -1;
  std::string dex_text;
  {
    char at;
    assert(!s.eof());
    s >> at;
    assert(!s.eof());
    while (!s.eof() && (at == '\n' || at == ' ')) { s >> at; }
    assert(!s.eof());
    assert(at == '@');
    char one;
    s >> one;
    assert(one == '1');
    char colon;
    s >> colon;
    assert(colon == ':');
    while (!s.eof())
    {
      char c;
      s >> c;
      if (c == '@') break;
      dex_text += c;
    }
    s >> dex_chapter;
  }
  assert(dex_chapter > -1);
  assert(!dex_text.empty());
  const bool has_dex{character.TestDexterity()};
  if (has_dex)
  {
    std::cout << dex_text << std::endl;
    chapter = dex_chapter;
  }
  else
  {
    std::cout << no_dex_text << std::endl;
    chapter = no_dex_chapter;
  }
}

void DoTestYourLuckChapter(std::stringstream& s, int& chapter, Character& character)
{
  //Parse no luck
  int no_luck_chapter = -1;
  std::string no_luck_text;
  {
    char at;
    assert(!s.eof());
    s >> at;
    assert(!s.eof());
    while (!s.eof() && (at == '\n' || at == ' ')) { s >> at; }
    assert(!s.eof());
    assert(at == '@');
    char zero;
    s >> zero;
    while (!s.eof() && (zero == '\n' || zero == ' '))
    {
      s >> zero;
    }
    assert(zero == '0');
    char colon;
    s >> colon;
    assert(colon == ':');
    while (!s.eof())
    {
      char c;
      s >> c;
      if (c == '@') break;
      no_luck_text += c;
    }
    s >> no_luck_chapter;
    assert(no_luck_chapter > -1);
    assert(!no_luck_text.empty());
  }

  //Parse luck
  int luck_chapter = -1;
  std::string luck_text;
  {
    char at;
    assert(!s.eof());
    s >> at;
    assert(!s.eof());
    while (!s.eof() && (at == '\n' || at == ' ')) { s >> at; }
    assert(!s.eof());
    assert(at == '@');
    char one;
    s >> one;
    assert(one == '1');
    char colon;
    s >> colon;
    assert(colon == ':');
    while (!s.eof())
    {
      char c;
      s >> c;
      if (c == '@') break;
      luck_text += c;
    }
    s >> luck_chapter;
  }
  assert(luck_chapter > -1);
  assert(!luck_text.empty());
  const bool has_luck{character.TestLuck()};
  if (has_luck)
  {
    std::cout << luck_text << std::endl;
    chapter = luck_chapter;
  }
  else
  {
    std::cout << no_luck_text << std::endl;
    chapter = no_luck_chapter;
  }
}
