#include "chapter.h"

#include <cassert>
#include <iterator>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "specialchapter.h"
#include "helper.h"

Chapter::Chapter(const std::string& filename)
  :
    m_bye_text{},
    m_consequence{},
    m_chapter_type{ChapterType::normal},
    m_fighting_chapter{},
    m_luck_chapter{},
    m_options_chapter{},
    m_pawn_shop_chapter{},
    m_shop_chapter{},
    m_skill_chapter{},
    m_text{}
{
  if (!IsRegularFile(filename))
  {
    std::stringstream msg;
    //msg << __func__ << ": ERROR: File " << filename << " does not exist";
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
    if (str.empty())
    {
      break;
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
    else if (str == "Change" || str == "change")
    {
      m_consequence.Add(ParseConsequence(s));
    }
    else if (str == "Escape" || str == "escape")
    {
      GetFighting().SetRoundsToEscape(ReadInt(s));
      GetFighting().SetEscapeToChapter(ReadInt(s));
    }
    else if (str == "Fight_both" || str == "fight_both")
    {
      GetFighting().SetFightSequentially(false);
    }
    else if (str == "Luck" || str == "luck")
    {
      this->m_chapter_type = ChapterType::test_your_luck;
      s << std::noskipws; //Obligatory
      //Parse(s,' '); //You expect a space after a word
      std::string luck_text;
      while (1)
      {
        char c = '*';
        s >> c;
        if (c == '@') break;
        luck_text += c;
      }
      s << std::skipws; //Obligatory
      assert(!luck_text.empty());
      GetLuck().SetLuckText(luck_text);
      const std::string goto_str{ReadString(s)};
      assert(goto_str == "goto");
      const int luck_chapter{ReadInt(s)};
      assert(luck_chapter > 1);
      GetLuck().SetLuckChapter(luck_chapter);
    }
    else if (str == "Monster" || str == "monster")
    {
      this->m_chapter_type = ChapterType::fight;
      const Monster monster{ParseMonster(s)};
      GetFighting().AddMonster(monster);
    }
    else if (str == "Next_chapter" || str == "goto")
    {
      m_consequence.SetNextChapter(ReadInt(s));
    }
    else if (str == "No_luck" || str == "no_luck")
    {
      s << std::noskipws; //Obligatory
      //Parse(s,' '); //You expect a space after a word
      std::string no_luck_text;
      while (1)
      {
        char c = '*';
        s >> c;
        if (c == '@') break;
        no_luck_text += c;
      }
      s << std::skipws; //Obligatory
      assert(!no_luck_text.empty());
      GetLuck().SetNoLuckText(no_luck_text);
      const std::string then{ReadString(s)};
      if (then == "change")
      {
        const Consequence no_luck_consequence{ParseConsequence(s)};
        GetLuck().SetNoLuckConsequence(no_luck_consequence);
        const std::string goto_str{ReadString(s)};
        assert(goto_str == "goto");
      }
      else
      {
        assert(then == "goto");
      }
      const int no_luck_chapter{ReadInt(s)};
      assert(no_luck_chapter > 1);
      GetLuck().SetNoLuckChapter(no_luck_chapter);
    }
    else if (str == "No_skill" || str == "no_skill")
    {
      s << std::noskipws; //Obligatory
      //Parse(s,' '); //You expect a space after a word
      std::string no_skill_text;
      while (1)
      {
        char c = '*';
        s >> c;
        if (c == '@') break;
        no_skill_text += c;
      }
      s << std::skipws; //Obligatory
      assert(!no_skill_text.empty());
      GetSkill().SetNoSkillText(no_skill_text);
      const std::string then_str{ReadString(s)};
      Consequence consequence;
      if (then_str == "goto")
      {
        consequence.SetNextChapter(ReadInt(s));
      }
      else if (then_str == "change")
      {
        consequence = ParseConsequence(s);
        //Also read goto
        const std::string goto_str{ReadString(s)};
        assert(goto_str == "goto");
        consequence.SetNextChapter(ReadInt(s));
      }
      else
      {
        assert(!"Should not get here");
      }
      GetSkill().SetNoSkillConsequence(consequence);
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
        const Condition condition{ParseCondition(s)};

        const std::string then_str{ReadString(s)};
        Consequence consequence;
        if (then_str == "goto")
        {
          consequence.SetNextChapter(ReadInt(s));
        }
        else if (then_str == "change")
        {
          consequence = ParseConsequence(s);
          //Also read goto
          const std::string goto_str{ReadString(s)};
          assert(goto_str == "goto");
          consequence.SetNextChapter(ReadInt(s));
        }
        else
        {
          assert(!"Should not get here");
        }
        Option option(option_text,consequence);
        option.SetCondition(condition);
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
          std::cerr << "Unknown item " << what << " in " << filename << std::endl;
          assert(!"Should not get here");
        }
        const std::string str_goto{ReadString(s)};
        assert(str_goto == "goto");
        Consequence consequence;
        consequence.SetNextChapter(ReadInt(s));
        Option option(option_text,consequence);
        option.SetCondition(condition);
        GetOptions().AddOption(option);
      }
      else if (t == "goto")
      {
        Consequence consequence;
        consequence.SetNextChapter(ReadInt(s));
        const Option option(option_text,consequence);
        GetOptions().AddOption(option);
      }
      else if (IsInt(t))
      {
        std::clog << "WARNING: goto omitted in file " << filename << std::endl;
        //If no goto, just parse the number
        Consequence consequence;
        consequence.SetNextChapter(std::stoi(t));
        const Option option(option_text,consequence);
        GetOptions().AddOption(option);
      }
      else
      {
        std::cerr << "Unknown option " << t << " in file " << filename <<std::endl;
        assert(!"Should not get here");
      }
    }
    else if (str == "Random_monsters" || str == "random_monsters")
    {
      std::vector<Monster> monsters{ParseMonsters(s)};
      m_chapter_type = ChapterType::fight;
      const int which_monster_index{ (std::rand() >> 4) % 6 };
      const Monster monster{monsters[which_monster_index]};
      m_fighting_chapter.AddMonster(monster);
    }
    else if (str == "Sell_items" || str == "sell_items")
    {
      m_chapter_type = ChapterType::pawn_shop;
      m_pawn_shop_chapter = ParsePawnShopChapter(s);
    }
    else if (str == "Shop_items" || str == "shop_items")
    {
      m_chapter_type = ChapterType::shop;
      m_shop_chapter = ParseShopChapter(s);
    }
    else if (str == "Skill" || str == "skill")
    {
      this->m_chapter_type = ChapterType::test_your_skill;
      s << std::noskipws; //Obligatory
      //Parse(s,' '); //You expect a space after a word
      std::string skill_text;
      while (1)
      {
        char c = '*';
        s >> c;
        if (c == '@') break;
        skill_text += c;
      }
      s << std::skipws; //Obligatory
      assert(!skill_text.empty());
      GetSkill().SetSkillText(skill_text);
      const std::string then_str{ReadString(s)};
      Consequence consequence;
      if (then_str == "goto")
      {
        consequence.SetNextChapter(ReadInt(s));
      }
      else if (then_str == "change")
      {
        consequence = ParseConsequence(s);
        //Also read goto
        const std::string goto_str{ReadString(s)};
        assert(goto_str == "goto");
        consequence.SetNextChapter(ReadInt(s));
      }
      else
      {
        assert(!"Should not get here");
      }
      GetSkill().SetSkillConsequence(consequence);

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

  if (GetType() == ChapterType::game_lost)
  {
    character.SetIsDead();
    return;
  }
  else if (GetType() == ChapterType::play_dice)
  {
    DoPlayDice(character,auto_play);
    m_consequence.Apply(character);
  }
  else if (GetType() == ChapterType::play_ball)
  {
    DoPlayBall(character,auto_play);
    m_consequence.Apply(character);
  }
  else if (GetType() == ChapterType::play_pill)
  {
    DoPlayPill(character,auto_play);
    if (character.IsDead()) return;
    std::cout << std::endl;
    m_consequence.Apply(character);
  }
  //Options
  else if (!GetOptions().GetOptions().empty())
  {
    if (GetOptions().GetValidOptions(character).empty())
    {
      std::cerr
        << "ERROR: no valid options in chapter " << character.GetCurrentChapter()
        << std::endl
        << "Options:\n"
      ;
      std::clog << "character.HasItem(Item::silver_arrow): " << character.HasItem(Item::silver_arrow) << std::endl;
      assert(GetOptions().GetOptions().size() == 2);
      //assert(GetOptions().GetOptions()[0].
      for (const auto option: GetOptions().GetOptions())
      {
        std::cerr << option << std::endl;
      }
    }
    while (1)
    {
      const auto options = GetOptions().GetValidOptions(character);
      assert(!options.empty());
      const int n_options{static_cast<int>(options.size())};
      for (int i=0; i!=n_options; ++i)
      {
        const auto option = options[i];
        std::cout << '[' << (i+1) << "]" << option.GetText() << std::endl;
      }
      std::cout << "[9] Status and inventory" << std::endl;
      //Chose an options
      if (auto_play)
      {
        std::cout << "AUTOPLAY: chose option #1" << std::endl;
        options[0].GetConsequence().Apply(character);
        //m_consequence = options[0].GetConsequence();
        //options[0].DoChoose(character);
        break;
      }
      //Only one option
      if (options.size() == 1)
      {
        options[0].GetConsequence().Apply(character);
        //options[0].DoChoose(character);
        break;
      }

      //Process command
      std::string s;
      std::getline(std::cin,s);
      if (s.empty()) continue;
      if (!IsInt(s))
      {
        std::cout << "Please enter a number" << std::endl;
        continue;
      }
      const int chosen_option_number{std::stoi(s)};
      if (chosen_option_number != 9
        && (chosen_option_number < 1 || chosen_option_number > static_cast<int>(options.size()))
      )
      {
        std::cout << "Please enter a number from 1 to " << options.size() << " or 9 for inventory" << std::endl;
        continue;
      }
      if (chosen_option_number == 9)
      {
        DoInventory(character,auto_play);
        std::cout << std::endl;
        continue;
      }
      const int chosen_option_index{chosen_option_number-1};
      assert(chosen_option_index >= 0);
      assert(chosen_option_index < static_cast<int>(options.size()));
      options[chosen_option_index].GetConsequence().Apply(character);
      //options[chosen_option_index].DoChoose(character);
      break;
    }
    assert(m_consequence.GetNextChapter() == -1);
    m_consequence.Apply(character);
  }
  else if (GetType() == ChapterType::fight)
  {
    m_fighting_chapter.Do(character,auto_play);
    assert(m_consequence.GetNextChapter() > 0);
    m_consequence.Apply(character);
  }
  else if (GetType() == ChapterType::test_your_luck)
  {
    GetLuck().Do(character,auto_play);
    //m_consequence.Apply(character); Applies its own consequences

  }
  else if (GetType() == ChapterType::test_your_skill)
  {
    GetSkill().Do(character,auto_play);
    //m_consequence.Apply(character); Applies its own consequences
  }
  else if (GetType() == ChapterType::shop)
  {
    GetShop().Do(character,auto_play);
    m_consequence.Apply(character);
  }
  else if (GetType() == ChapterType::pawn_shop)
  {
    GetPawnShop().Do(character,auto_play);
    m_consequence.Apply(character);
  }
  else if (GetType() == ChapterType::normal)
  {
    //Nothing
    m_consequence.Apply(character);
  }
  else
  {
    assert(!"Should not get here");
  }

  if (character.IsDead()) return;

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
