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
  : m_add_items{},
    m_bye_text{},
    m_change_gold{0},
    m_change_luck{0},
    m_chapter_type{ChapterType::normal},
    m_escape_chapter{-1},
    m_fight_sequentially{true},
    m_monsters{},
    m_next_chapter{-1},
    m_rounds_to_escape{1000},
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
  if (!(
       chapter_type == 5
    || chapter_type == 7
    || chapter_type == 10
    || chapter_type == 11
    || chapter_type == 14 //dice game
    || chapter_type == 15 //pill game
    || chapter_type == 999
  ))
  {
    throw std::logic_error("Chapter type not yet supported");
  }
  s << std::skipws; //Now strings will be read

  if (chapter_type == 5)
  {
    m_chapter_type = ChapterType::game_lost;
    return;
  }
  if (chapter_type == 11)
  {
    m_chapter_type = ChapterType::game_won;
    return;
  }
  if (chapter_type == 14)
  {
    m_chapter_type = ChapterType::play_dice;
  }
  if (chapter_type == 15)
  {
    m_chapter_type = ChapterType::play_pill;
  }

  while (!s.eof())
  {
    const std::string str{ReadString(s)};
    if (str.empty()) break;
    if (str == "Fight_both")
    {
      m_fight_sequentially = false;
    }
    else if (str == "Bye")
    {
      while (1)
      {
        const char c{ReadChar(s)};
        if (c == '@') break;
        m_bye_text += c;
      }
    }
    else if (str == "Monster")
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
    else if (str == "Escape")
    {
      m_rounds_to_escape = ReadInt(s);
      assert(m_rounds_to_escape >= 0);
      m_escape_chapter = ReadInt(s);
      assert(m_escape_chapter > 0);
    }
    else if (str == "Change")
    {
      const std::string what{ReadString(s)};
      if (what == "gold")
      {
        const int change_gold{ReadInt(s)};
        m_change_gold = change_gold;
      }
      else if (what == "luck")
      {
        const int change_luck{ReadInt(s)};
        m_change_luck = change_luck;
      }
      else if (what == "add")
      {
        const std::string item_name{ReadString(s)};
        const Item item{ToItem(item_name)};
        m_add_items.insert(item);
      }
    }
    else
    {
      std::cerr << "Chapter cannot parse " << filename << std::endl;
      assert(!"Should not get here");
    }
  }
}

void Chapter::Do(Character& character,const bool auto_play) const
{
  //Display the text line by line
  ShowText(m_text,auto_play);

  //Fight
  if (!m_monsters.empty())
  {
    std::cout << std::endl;
    if (m_fight_sequentially)
    {
      DoFight(m_monsters,character,auto_play);
    }
    else
    {
      DoFightTwoMonsters(m_monsters,character,auto_play);
    }
    assert(m_next_chapter > 0);
    character.SetChapter(m_next_chapter);
  }

  if (GetType() == ChapterType::game_lost) { character.SetIsDead(); }
  if (GetType() == ChapterType::play_dice)
  {
    DoPlayDice(character,auto_play);
    ShowText(m_bye_text,auto_play);
    character.SetChapter(m_next_chapter);
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
