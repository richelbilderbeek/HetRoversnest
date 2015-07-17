#include "chapter.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <boost/lexical_cast.hpp>

#include "dochapter.h"
#include "helper.h"

Chapter::Chapter(const std::string& filename)
  :
    m_bye_text{},
    m_consequence{},
    m_chapter_type{ChapterType::normal},
    m_fighting_chapter{},
    m_next_chapter{-1},
    m_options_chapter{},
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
    || chapter_type == 16 //ball game
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
  if (chapter_type == 16)
  {
    m_chapter_type = ChapterType::play_ball;
  }

  while (!s.eof())
  {
    const std::string str{ReadString(s)};
    if (str.empty()) break;
    if (str == "Fight_both")
    {
      GetFighting().SetFightSequentially(false);
    }
    else if (str == "Bye" || str == "bye")
    {
      s << std::noskipws; //Obligatory
      //Parse(s,' '); //You expect a space after a word
      while (1)
      {
        char c = '*';
        s >> c;
        if (c == '@') break;
        m_bye_text += c;
      }
      s << std::skipws; //Obligatory
    }
    else if (str == "Monster" || str == "monster")
    {
      const std::string name{ReadString(s)};
      const int dexterity{ReadInt(s)};
      const int condition{ReadInt(s)};
      const int attack_strength{ReadInt(s)};
      const Monster monster(name,dexterity,condition,attack_strength);
      GetFighting().AddMonster(monster);
    }
    else if (str == "Next_chapter" || str == "goto")
    {
      m_next_chapter = ReadInt(s);
    }
    else if (str == "Escape" || str == "escape")
    {
      GetFighting().SetRoundsToEscape(ReadInt(s));
      GetFighting().SetEscapeToChapter(ReadInt(s));
    }
    else if (str == "Option" || str == "option")
    {
      std::string option_text;
      s << std::noskipws; //Obligatory
      //Parse(s,' '); //You expect a space after a word
      while (1)
      {
        char c = '*';
        s >> c;
        if (c == '@') break;
        option_text += c;
      }
      s << std::skipws; //Obligatory
      const std::string t{ReadString(s)};
      if (t == "if")
      {
        Condition condition;
        const std::string what{ReadString(s)};

        if (what == "gold")
        {
          const int gold_amount{ReadInt(s)};
          condition.SetGoldNeeded(gold_amount);
        }
        else if (what == "has")
        {
          const std::string item = ReadString(s);
          if (!IsItem(item))
          {
            std::cerr << "Unknown item " << item << " in " << filename << std::endl;
            assert(!"Should not get here");
          }
          const Item item_needed{ToItem(item)};
          condition.AddItemNeeded(item_needed);
        }
        else if (what == "has_not")
        {
          const std::string item = ReadString(s);
          if (!IsItem(item))
          {
            std::cerr << "Unknown item " << item << " in " << filename << std::endl;
            assert(!"Should not get here");
          }
          const Item item_not_needed{ToItem(item)};
          condition.AddItemNotNeeded(item_not_needed);
        }
        else if (IsItem(what)) //assumes item is needed, 'has' before it can be omitted
        {
          const Item item_needed{ToItem(what)};
          condition.AddItemNeeded(item_needed);
        }
        else
        {
          std::cerr << "Unknown option after if in " << filename << std::endl;
          assert(!"Should not get here");
        }
        const std::string str_goto{ReadString(s)};
        assert(str_goto == "goto");
        const int option_next_chapter{ReadInt(s)};
        Option option(option_text,option_next_chapter);
        option.AddCondition(condition);
        GetOptions().AddOption(option);
      }
      else if (t == "ifnot")
      {
        Condition condition;
        const std::string what{ReadString(s)};
        if (IsItem(what))
        {
          const Item item_not_needed{ToItem(what)};
          condition.AddItemNotNeeded(item_not_needed);
        }
        else
        {
          assert(!"Should not get here");
        }
        const std::string str_goto{ReadString(s)};
        assert(str_goto == "goto");
        const int option_next_chapter{ReadInt(s)};
        Option option(option_text,option_next_chapter);
        option.AddCondition(condition);
        GetOptions().AddOption(option);
      }
      else if (t == "goto")
      {
        const int option_next_chapter{ReadInt(s)};
        const Option option(option_text,option_next_chapter);
        GetOptions().AddOption(option);
      }
      else
      {
        assert(!"Should not get here");
      }
    }
    else if (str == "Change" || str == "change")
    {

      const std::string what{ReadString(s)};
      if (what == "gold")
      {
        const int change_gold{ReadInt(s)};
        m_consequence.SetChangeGold(change_gold);
      }
      else if (what == "dexterity" || what == "dex")
      {
        const int change_dex{ReadInt(s)};
        m_consequence.SetChangeDexterity(change_dex);
      }
      else if (what == "luck")
      {
        const int change_luck{ReadInt(s)};
        m_consequence.SetChangeLuck(change_luck);
      }
      else if (what == "stamina" || what == "sta")
      {
        const int change_sta{ReadInt(s)};
        m_consequence.SetChangeStamina(change_sta);
      }
      else if (what == "add")
      {
        const std::string item_name{ReadString(s)};
        if (!IsItem(item_name))
        {
          std::cerr << "Unknown item: " << item_name << std::endl;
          assert(!"Should not get here");
        }
        const Item item{ToItem(item_name)};
        m_consequence.AddItemToAdd(item);
      }
      else if (what == "remove")
      {
        const std::string item_name{ReadString(s)};
        if (!IsItem(item_name))
        {
          std::cerr << "Unknown item: " << item_name << std::endl;
          assert(!"Should not get here");
        }
        const Item item{ToItem(item_name)};
        m_consequence.AddItemToRemove(item);
      }
      else
      {
        std::cerr << "Unknown what: " << what << std::endl;
        assert(!"Should not get here");
      }
    }
    else
    {
      std::cerr
        << "Chapter cannot parse " << filename  << '\n'
        << "Unknown string: " << str << '\n'
      ;
      assert(!"Should not get here");
    }
  }
}

void Chapter::Do(Character& character,const bool auto_play) const
{
  //Display the text line by line
  ShowText(m_text,auto_play);

  std::cout << std::endl;

  m_consequence.Apply(character);

  //Options
  if (!GetOptions().GetOptions().empty())
  {
    const auto options = GetOptions().GetValidOptions(character);
    const int n_options{static_cast<int>(options.size())};
    for (int i=0; i!=n_options; ++i)
    {
      const auto option = options[i];
      std::cout << '[' << (i+1) << "]" << option.GetText() << std::endl;
    }
    //Chose an options
    if (auto_play)
    {
      std::cout << "AUTOPLAY: chose option #1" << std::endl;
      options[0].DoChoose(character);
      return;
    }
    //Only one option
    if (options.size() == 1)
    {
      options[0].DoChoose(character);
      return;
    }
    //Process command
    while (1)
    {
      std::string s;
      std::getline(std::cin,s);
      if (s.empty()) continue;
      try { boost::lexical_cast<int>(s); }
      catch (boost::bad_lexical_cast&)
      {
        std::cout << "Please enter a number" << std::endl;
        continue;
      }
      const int chosen_option_number{boost::lexical_cast<int>(s)};
      const int chosen_option_index{chosen_option_number-1};
      assert(chosen_option_index >= 0);
      assert(chosen_option_index < static_cast<int>(options.size()));
      options[chosen_option_index].DoChoose(character);
      break;
    }
  }
  else if (!GetFighting().GetMonsters().empty())
  {
    if (GetFighting().DoFightSequentially())
    {
      DoFight(GetFighting().GetMonsters(),character,auto_play);
    }
    else
    {
      DoFightTwoMonsters(GetFighting().GetMonsters(),character,auto_play);
    }
    assert(m_next_chapter > 0);
    character.SetChapter(m_next_chapter);
  }
  else if (GetType() == ChapterType::game_lost)
  {
    character.SetIsDead();
    return;
  }
  else if (GetType() == ChapterType::play_dice)
  {
    DoPlayDice(character,auto_play);
    character.SetChapter(m_next_chapter);
  }
  else if (GetType() == ChapterType::play_ball)
  {
    DoPlayBall(character,auto_play);
    character.SetChapter(m_next_chapter);
  }
  else if (GetType() == ChapterType::play_pill)
  {
    DoPlayPill(character,auto_play);
    character.SetChapter(m_next_chapter);
    if (character.IsDead()) return;
    std::cout << std::endl;
  }
  else if (GetType() == ChapterType::normal)
  {
    character.SetChapter(m_next_chapter);
  }
  ShowText(m_bye_text,auto_play);
}

std::ostream& operator<<(std::ostream& os, const Chapter& chapter)
{
  os
    << "text: " << chapter.GetText() << '\n'
    << "# monsters: " << chapter.GetFighting().GetMonsters().size() << '\n'
  ;
  for (const auto monster: chapter.GetFighting().GetMonsters())
  {
    os << monster << '\n';
  }
  os
    << "next_chapter: " << chapter.GetNextChapter() << '\n'
  ;
  return os;
}
