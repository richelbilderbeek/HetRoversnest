#include "chapter.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "dochapter.h"
#include "helper.h"

Chapter::Chapter(const std::string& filename)
  : m_monsters{},
    m_next_chapter{0},
    m_text{}
{
  if (!IsRegularFile(filename))
  {
    std::stringstream msg;
    msg << __func__ << ": ERROR: File " << filename << " does not exist";
    throw std::runtime_error(msg.str());
  }
  const std::vector<std::string> lines = FileToVector(filename);
  std::stringstream s;
  s << std::noskipws; //Obligatory
  std::copy(std::begin(lines),std::end(lines),std::ostream_iterator<std::string>(s," "));

  //Show text until @
  {
    std::stringstream text;
    int pos = 0;
    char prev_c = ' ';
    while (s)
    {
      char c;
      s >> c;
      if (c == '@')
      {
        s.putback('@');
        break; //Now the options must be parsed
      }
      if (c == '\n') c = ' '; //Convert a newline to space, so input files do not require a space after every line
      if (c == ' ' && pos == 0) continue; //Een nieuwe regel begint niet met een spatie
      if (c == ' ' && prev_c == ' ') continue; //Tweede spatie overslaan
      if (c == '[') { text << '\n'; pos = 0; }
      text << c;
      prev_c = c;
      ++pos;
      if (pos > 60 && c == ' ')
      {
        text << '\n';
        pos = 0;
      }
    }
    m_text = text.str();
  }

  Parse(s,'@');
  const int chapter_type = ReadInt(s);
  assert(chapter_type == 999);

  s << std::skipws; //Now strings will be read

  while (!s.eof())
  {
    const std::string str{ReadString(s)};
    if (str.empty()) break;
    if (str == "Monster")
    {
      const std::string name{ReadString(s)};
      const int dexterity{ReadInt(s)};
      const int condition{ReadInt(s)};
      const int attack_strength{ReadInt(s)};
      const Monster monster(name,dexterity,condition,attack_strength);
      m_monsters.push_back(monster);
    }
    else if (str == "Next_chapter")
    {
      m_next_chapter = ReadInt(s);
    }
    else
    {
      assert(!"Should not get here");
    }
  }
}

std::ostream& operator<<(std::ostream& os, const Chapter& chapter)
{
  os
    << "text: " << chapter.GetText() << '\n'
    << "# monsters: " << chapter.GetMonsters().size() << '\n'
  ;
  for (const auto monster: chapter.GetMonsters())
  {
    os << monster << '\n';
  }
  os
    << "next_chapter: " << chapter.GetNextChapter() << '\n'
  ;
  return os;
}
