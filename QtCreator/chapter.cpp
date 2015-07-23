#include "chapter.h"

#include <cassert>
#include <iterator>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "ai.h"
#include "dice.h"
#include "specialchapter.h"
#include "helper.h"

Chapter::Chapter(const int chapter_number)
  :
    m_signal_request_input{},
    m_signal_show_text{},
    m_signal_wait{},
    m_ball_game_chapter{*this},
    m_bye_text{},
    m_consequence{},
    m_chapter_number{chapter_number},
    m_chapter_type{ChapterType::normal},
    m_dice_game_chapter{*this},
    m_fighting_chapter{FightingChapter(*this)},
    m_luck_chapter(*this),
    m_options_chapter{},
    m_pawn_shop_chapter(this),
    m_pill_game_chapter{*this},
    m_shop_chapter{this},
    m_skill_chapter{*this},
    m_text{}
{
  #ifndef NDEBUG
  Test();
  #endif

  const std::string filename{"../Files/" + std::to_string(chapter_number) + ".txt"};
  if (!IsRegularFile(filename))
  {
    std::stringstream msg;
    msg << __func__ << ": ERROR: File " << filename << " does not exist";
    throw std::runtime_error(msg.str());
  }
  if (!IsRegularFile(filename))
  {
    std::stringstream msg;
    //msg << __func__ << ": ERROR: File " << filename << " does not exist";
    throw std::runtime_error(msg.str());
  }
  const std::vector<std::string> lines = FileToVector(filename);
  std::stringstream s;
  std::copy(std::begin(lines),std::end(lines),std::ostream_iterator<std::string>(s," "));

  m_text = ReadText(s);

  //Parse(s,'@'); //Read by ReadText
  try
  {
    m_chapter_type = ReadChapterType(s);
  }
  catch (std::runtime_error& e)
  {
    m_chapter_type = ChapterType::normal;
    std::cerr << "WARNING: error in parsing " << filename << std::endl;
  }
  //const std::string chapter_type_str{ReadString(s)};
  //const int chapter_type = ReadInt(s);

  switch (m_chapter_type)
  {
    case ChapterType::game_lost: return;
    case ChapterType::game_won: return;
    default: break;
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
      assert(this->m_chapter_type == ChapterType::test_your_luck);
      const std::string luck_text{ReadText(s)};
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
      assert(this->m_chapter_type == ChapterType::test_your_luck);
      //s << std::noskipws; //Obligatory
      //Parse(s,' '); //You expect a space after a word
      const std::string no_luck_text{ReadText(s)};
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
      assert(this->m_chapter_type == ChapterType::test_your_skill);
      //Parse(s,' '); //You expect a space after a word
      const std::string no_skill_text{ReadText(s)};
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
      const std::string option_text{ReadText(s)};
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
      const int which_monster_index{Dice::Get()->Throw() - 1};
      assert(which_monster_index >= 0);
      assert(which_monster_index < static_cast<int>(monsters.size()));
      const Monster monster{monsters[which_monster_index]};
      m_fighting_chapter.AddMonster(monster);
    }
    else if (str == "Sell_items" || str == "sell_items")
    {
      assert(this->m_chapter_type == ChapterType::pawn_shop);
      //m_chapter_type = ChapterType::pawn_shop;
      m_pawn_shop_chapter = ParsePawnShopChapter(s,this);
    }
    else if (str == "Shop_items" || str == "shop_items")
    {
      assert(this->m_chapter_type == ChapterType::shop);
      //m_chapter_type = ChapterType::shop;
      m_shop_chapter = ParseShopChapter(s,this);
    }
    else if (str == "Skill" || str == "skill")
    {
      assert(this->m_chapter_type == ChapterType::test_your_skill);
      this->m_chapter_type = ChapterType::test_your_skill;
      //Parse(s,' '); //You expect a space after a word
      const std::string skill_text{ReadText(s)};
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

void Chapter::Do(Character& character) const
{
  assert(m_signal_request_input.num_slots() > 0);
  assert(m_signal_wait.num_slots() > 0);
  assert(m_signal_request_input.num_slots() > 0);

  #ifndef NDEBUG
  m_signal_show_text(
      "\n"
    + std::string(60,'-') + "\n"
    + std::to_string(GetChapterNumber()) + "\n"
    + std::string(60,'-') + "\n"
  );
  #endif

  //Display the text line by line
  m_signal_show_text(m_text + "\n");

  if (GetType() == ChapterType::game_lost)
  {
    character.SetIsDead();
    return;
  }
  else if (GetType() == ChapterType::game_won)
  {
    return;
  }
  else if (GetType() == ChapterType::play_dice)
  {
    m_dice_game_chapter.Do(character);
    m_consequence.Apply(character);
  }
  else if (GetType() == ChapterType::play_ball)
  {
    m_ball_game_chapter.Do(character);
    m_consequence.Apply(character);
  }
  else if (GetType() == ChapterType::play_pill)
  {
    m_pill_game_chapter.Do(character);
    if (character.IsDead()) return;
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
      //Show options
      const auto options = GetOptions().GetValidOptions(character);
      assert(!options.empty());
      const int n_options{static_cast<int>(options.size())};
      std::vector<int> user_options;
      {
        std::stringstream text;
        for (int i=0; i!=n_options; ++i)
        {
          const auto option = options[i];
          text << '[' << (i+1) << "] " << option.GetText() << '\n';
          user_options.push_back(i+1);
        }
        text << "[0] Status and inventory\n";
        user_options.push_back(0);
        m_signal_show_text(text.str());
      }
      //Only one option
      if (options.size() == 1)
      {
        options[0].GetConsequence().Apply(character);
        break;
      }

      //Process command
      const int chosen_option_number{*m_signal_request_input(user_options)};
      if (chosen_option_number != 0
        && (chosen_option_number < 1 || chosen_option_number > static_cast<int>(options.size()))
      )
      {
        std::stringstream text;
        text << "Please enter a number from 1 to " << options.size() << " or 0 for inventory\n";
        m_signal_show_text(text.str());
        continue;
      }
      if (chosen_option_number == 0)
      {
        m_signal_show_text(character.ShowInventory() + "\n");
        continue;
      }
      const int chosen_option_index{chosen_option_number-1};
      assert(chosen_option_index >= 0);
      assert(chosen_option_index < static_cast<int>(options.size()));
      options[chosen_option_index].GetConsequence().Apply(character);
      break;
    }
    assert(m_consequence.GetNextChapter() == -1);
    m_consequence.Apply(character);
  }
  else if (GetType() == ChapterType::fight)
  {
    m_fighting_chapter.Do(character);
    assert(m_consequence.GetNextChapter() > 0);
    m_consequence.Apply(character);
  }
  else if (GetType() == ChapterType::test_your_luck)
  {
    m_signal_show_text("Test your luck...\n");
    GetLuck().Do(character);
    //m_consequence.Apply(character); Applies its own consequences

  }
  else if (GetType() == ChapterType::test_your_skill)
  {
    m_signal_show_text("Test your skill...\n");
    GetSkill().Do(character);
    //m_consequence.Apply(character); Applies its own consequences
  }
  else if (GetType() == ChapterType::shop)
  {
    GetShop().Do(character);
    m_consequence.Apply(character);
  }
  else if (GetType() == ChapterType::pawn_shop)
  {
    GetPawnShop().Do(character);
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

  m_signal_show_text(m_bye_text);
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
