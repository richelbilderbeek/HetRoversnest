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

void DoChangeStatusChapter(std::stringstream& s, int& chapter, Character& character)
{
  const bool verbose{false};
  if (verbose) { std::clog << "CHAPTER " << chapter << std::endl; }
  //Parse @ starting what
  {
    char at;
    assert(!s.eof());
    s >> at;
    assert(!s.eof());
    while (!s.eof() && (at == '\n' || at == ' ')) { s >> at; }
    assert(!s.eof());
    assert(at == '@');
  }
  while (1)
  {
    assert(!s.eof());
    char status = '*';
    s >> status;
    assert(status != '*');
    if (verbose) { std::clog << "status: " << status << std::endl; }
    if (status == 'I')
    {
      int number = -1;
      s >> number;
      assert(number > -1);
      const Item item = static_cast<Item>(number);
      char plus_or_minus_or_question_mark = '*';
      s >> plus_or_minus_or_question_mark;
      if (plus_or_minus_or_question_mark == '+')
      {
        if (verbose) { std::clog << "add item: " << item << std::endl; }
        character.AddItem(item);
      }
      else if (plus_or_minus_or_question_mark == '-')
      {
        if (verbose) { std::clog << "remove item: " << item << std::endl; }
        character.RemoveItem(item);
      }
      else if (plus_or_minus_or_question_mark == '?')
      {
        char conditional_status = '*';
        s >> conditional_status;
        assert(conditional_status != '*');
        if (verbose) { std::clog << "conditional_status: " << conditional_status << std::endl; }
        if (conditional_status == 'I')
        {
          int conditional_number = -1;
          s >> conditional_number;
          assert(conditional_number > -1);
          const Item conditional_item = static_cast<Item>(conditional_number);
          char conditional_plus_or_minus_or_question_mark = '*';
          s >> conditional_plus_or_minus_or_question_mark;
          if (conditional_plus_or_minus_or_question_mark == '+')
          {
            if (character.HasItem(item))
            {
              if (verbose) { std::clog << "player has item: " << item << std::endl; }
              if (verbose) { std::clog << "add conditional_item: " << conditional_item << std::endl; }
              character.AddItem(conditional_item);
            }
            else
            {
              if (verbose) { std::clog << "player does not have item: " << item << std::endl; }
            }
          }
          else if (conditional_plus_or_minus_or_question_mark == '-')
          {
            if (character.HasItem(item))
            {
              if (verbose) { std::clog << "player has item: " << item << std::endl; }
              if (verbose) { std::clog << "remove conditional_item: " << conditional_item << std::endl; }
              character.RemoveItem(conditional_item);
            }
            else
            {
              if (verbose) { std::clog << "player does not have item: " << item << std::endl; }
            }
          }
        }
        else
        {
          int conditional_change = -123;
          s >> conditional_change;
          assert(conditional_change != -123);
          if (character.HasItem(item))
          {
            if (verbose) { std::clog << "player has item: " << item << std::endl; }
            switch (conditional_status)
            {
              case 'D':
                if (verbose) { std::clog << "Change dexterity by " << conditional_change << std::endl; }
                character.ChangeDexterity(conditional_change);
              break;
              case 'S':
                if (verbose) { std::clog << "Change stamina by " << conditional_change << std::endl; }
                character.ChangeStamina(conditional_change);
              break;
              case 'L':
                if (verbose) { std::clog << "Change luck by " << conditional_change << std::endl; }
                character.ChangeLuck(conditional_change);
              break;
              default: assert(!"Should not get here");
            }
          }
          else
          {
            if (verbose) { std::clog << "player does not have item: " << item << std::endl; }
          }
        }
      }
      else
      {
        assert(!"Should not get here");
      }
    }
    else
    {
      //Status is changed
      int change = -123;
      s >> change;
      assert(change != -123);
      switch (status)
      {
        case 'D':
          if (verbose) { std::clog << "Change dexterity by " << change << std::endl; }
          character.ChangeDexterity(change);
        break;
        case 'S':
          if (verbose) { std::clog << "Change stamina by " << change << std::endl; }
          character.ChangeStamina(change);
        break;
        case 'L':
          if (verbose) { std::clog << "Change luck by " << change << std::endl; }
          character.ChangeLuck(change);
        break;
        default: assert(!"Should not get here");
      }
    }
    char comma_or_at = '*';
    s >> comma_or_at;
    assert(!s.eof());
    while (!s.eof() && (comma_or_at == '\n' || comma_or_at == ' ')) { s >> comma_or_at; }
    if (comma_or_at == '@') break;
  }
  //Parse next chapter
  {
    int next_chapter = -1;
    s >> next_chapter;
    assert(next_chapter > -1);
    chapter = next_chapter;
  }
}



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
    case '3': DoChangeStatusChapter(s,chapter,character); break;
    case '4': DoFightWithTime(s,chapter,character,auto_play); break;
    case '5': DoGameOver(chapter); break;
    case '6': DoHasItemChapter(s,chapter,character); break;
    case '7': DoFight(s,chapter,character,auto_play); break;
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

void DoFight(std::stringstream& s, int& chapter, Character& character, const bool auto_play)
{
  //Name monster
  std::string name;
  {
    char at;
    assert(!s.eof());
    s >> at;
    assert(!s.eof());
    while (!s.eof() && (at == '\n' || at == ' ')) { s >> at; }
    assert(!s.eof());
    assert(at == '@');
    while (1)
    {
      char c = '*';
      s >> c;
      assert(c != '*');
      if (c == '@') break;
      name += c;
    }
  }
  //Skill monster
  int monster_dexterity = -1;
  {
    //char at;
    //assert(!s.eof());
    //s >> at;
    //assert(!s.eof());
    //while (!s.eof() && (at == '\n' || at == ' ')) { s >> at; }
    //assert(!s.eof());
    //assert(at == '@');
    s >> monster_dexterity;
    assert(monster_dexterity != -1);
  }
  //Condition monster
  int monster_stamina = -1;
  {
    char at;
    assert(!s.eof());
    s >> at;
    assert(!s.eof());
    while (!s.eof() && (at == '\n' || at == ' ')) { s >> at; }
    assert(!s.eof());
    assert(at == '@');
    s >> monster_stamina;
    assert(monster_stamina != -1);
  }
  //New chapter
  int new_chapter = -1;
  {
    char at;
    assert(!s.eof());
    s >> at;
    assert(!s.eof());
    while (!s.eof() && (at == '\n' || at == ' ')) { s >> at; }
    assert(!s.eof());
    assert(at == '@');
    s >> new_chapter;
    assert(new_chapter > -1);
  }
  while (1)
  {
    if (character.GetStamina() < 1) break;
    if (monster_stamina < 1) break;
    const int monster_attack = (std::rand() >> 4) % 6;
    const int player_attack = (std::rand() >> 4) % 12;
    if (player_attack > monster_attack)
    {
      std::cout << "You hit the monster." << std::endl;
      if (!auto_play)
      {
        std::cout << "Do you want to use luck?" << std::endl;
        assert(!"TODO");
      }
      else
      {
        monster_stamina -= 2;
      }
    }
    else if (player_attack < monster_attack)
    {
      std::cout << "You were hit by the monster." << std::endl;
      if (!auto_play)
      {
        std::cout << "Do you want to use luck?" << std::endl;
        assert(!"TODO");
      }
      else
      {
        character.ChangeStamina(-2);
      }
    }
    else
    {
      std::cout << "No damage was dealt." << std::endl;
    }
  }

  if (character.GetStamina() == 0)
  {
    std::cout
      << "The monster defeated you.\n"
      << "\n"
      << "*************\n"
      << "*           *\n"
      << "* GAME OVER *\n"
      << "*           *\n"
      << "*************\n"
    ;
    chapter = 0;
  }
  else
  {
    std::cout << "You defeated the monster." << std::endl;
    chapter = new_chapter;
  }
}

void DoFightWithTime(std::stringstream& s, int& chapter, Character& character, const bool auto_play)
{
  const bool verbose{false};
  //Name monster
  std::string name;
  {
    char at;
    assert(!s.eof());
    s >> at;
    assert(!s.eof());
    while (!s.eof() && (at == '\n' || at == ' ')) { s >> at; }
    assert(!s.eof());
    assert(at == '@');
    while (1)
    {
      char c = '*';
      s >> c;
      assert(c != '*');
      if (c == ' ' || c == '\n') continue;
      if (c == '@') break;
      name += c;
    }
  }
  if (verbose) { std::cout << "Monster name: " << name << std::endl; }
  //Skill monster
  int monster_dexterity = -1;
  {
    //char at;
    //assert(!s.eof());
    //s >> at;
    //assert(!s.eof());
    //while (!s.eof() && (at == '\n' || at == ' ')) { s >> at; }
    //assert(!s.eof());
    //assert(at == '@');
    s >> monster_dexterity;
    assert(monster_dexterity != -1);
  }
  if (verbose) { std::cout << "Monster dexterity: " << monster_dexterity << std::endl; }
  //Condition monster
  int monster_stamina = -1;
  {
    char at;
    assert(!s.eof());
    s >> at;
    assert(!s.eof());
    while (!s.eof() && (at == '\n' || at == ' ')) { s >> at; }
    assert(!s.eof());
    assert(at == '@');
    s >> monster_stamina;
    assert(monster_stamina != -1);
  }
  if (verbose) { std::cout << "Monster stamina: " << monster_stamina << std::endl; }
  //Number of rounds
  int number_of_rounds = -1;
  {
    char at;
    assert(!s.eof());
    s >> at;
    assert(!s.eof());
    while (!s.eof() && (at == '\n' || at == ' ')) { s >> at; }
    assert(!s.eof());
    assert(at == '@');
    s >> number_of_rounds;
    assert(number_of_rounds > -1);
  }
  if (verbose) { std::cout << "Number of rounds: " << number_of_rounds << std::endl; }
  //New chapter after time limit
  int new_chapter_after_time_limit = -1;
  {
    char at;
    assert(!s.eof());
    s >> at;
    assert(!s.eof());
    while (!s.eof() && (at == '\n' || at == ' ')) { s >> at; }
    assert(!s.eof());
    assert(at == '@');
    s >> new_chapter_after_time_limit;
    assert(new_chapter_after_time_limit > -1);
  }
  if (verbose) { std::cout << "New chapter after time limit: " << new_chapter_after_time_limit << std::endl; }
  //New chapter after time limit
  int new_chapter_within_time_limit = -1;
  {
    char at;
    assert(!s.eof());
    s >> at;
    assert(!s.eof());
    while (!s.eof() && (at == '\n' || at == ' ')) { s >> at; }
    assert(!s.eof());
    assert(at == '@');
    s >> new_chapter_within_time_limit;
    assert(new_chapter_within_time_limit > -1);
  }
  if (verbose) { std::cout << "New chapter within time limit: " << new_chapter_within_time_limit << std::endl; }
  for (int i=0; i!=number_of_rounds; ++i)
  {
    const int monster_attack = (std::rand() >> 4) % 6;
    const int player_attack = (std::rand() >> 4) % 12;
    if (player_attack > monster_attack)
    {
      std::cout << "You hit the monster." << std::endl;
      if (!auto_play)
      {
        std::cout << "Do you want to use luck?" << std::endl;
        assert(!"TODO");
      }
      else
      {
        monster_stamina -= 2;
      }
    }
    else if (player_attack < monster_attack)
    {
      std::cout << "You were hit by the monster." << std::endl;
      if (!auto_play)
      {
        std::cout << "Do you want to use luck?" << std::endl;
        assert(!"TODO");
      }
      else
      {
        character.ChangeStamina(-2);
      }
    }
    else
    {
      std::cout << "No damage was dealt." << std::endl;
    }
    if (character.GetStamina() < 1)
    {
      std::cout
        << "The monster defeated you.\n"
        << "\n"
        << "*************\n"
        << "*           *\n"
        << "* GAME OVER *\n"
        << "*           *\n"
        << "*************\n"
      ;
      chapter = 0;
      return;
    }
    else if (monster_stamina < 1)
    {
      std::cout << "You defeated the monster." << std::endl;
      chapter = new_chapter_within_time_limit;
      return;
    }
  }
  std::cout << "You did not make it within the time limit." << std::endl;
  chapter = new_chapter_after_time_limit;
}

void DoGameOver(int& chapter)
{
  std::cout
    << "*************\n"
    << "*           *\n"
    << "* GAME OVER *\n"
    << "*           *\n"
    << "*************\n"
  ;
  chapter = 0;
}

void DoHasItemChapter(std::stringstream& s, int& chapter, Character& character)
{
  const bool verbose{false};
  if (verbose) { std::clog << "CHAPTER " << chapter << std::endl; }
  std::vector<int> item_numbers;
  //Parse item
  {
    char at;
    assert(!s.eof());
    s >> at;
    assert(!s.eof());
    while (!s.eof() && (at == '\n' || at == ' ')) { s >> at; }
    assert(!s.eof());
    assert(at == '@');
    while (1)
    {
      int item_number = -1;
      s >> item_number;
      assert(item_number != -1);
      item_numbers.push_back(item_number);
      if (verbose) { std::clog << "Needed item: " << static_cast<Item>(item_number) << std::endl; }
      char comma_or_at = '*';
      s >> comma_or_at;
      assert(comma_or_at != '*');
      if (comma_or_at != ',') break;
    }
  }
  assert(!item_numbers.empty());
  int chapter_if_not_have = -1;
  //Parse chapter if not have
  {
    char at;
    assert(!s.eof());
    s >> at;
    assert(!s.eof());
    while (!s.eof() && (at == '\n' || at == ' ')) { s >> at; }
    assert(!s.eof());
    assert(at == '@');
    s >> chapter_if_not_have;
    if (verbose) { std::clog << "chapter_if_not_have: " << chapter_if_not_have << std::endl; }
  }
  assert(chapter_if_not_have != -1);
  int chapter_if_have = -1;
  //Parse chapter if have
  {
    char at;
    assert(!s.eof());
    s >> at;
    assert(!s.eof());
    while (!s.eof() && (at == '\n' || at == ' ')) { s >> at; }
    assert(!s.eof());
    assert(at == '@');
    s >> chapter_if_have;
    if (verbose) { std::clog << "chapter_if_have: " << chapter_if_have << std::endl; }
  }
  assert(chapter_if_have != -1);
  for (const int item_number: item_numbers)
  {
    const Item item = static_cast<Item>(item_number);
    if (!character.HasItem(item))
    {
      chapter = chapter_if_not_have;
      return;
    }
  }
  chapter = chapter_if_have;
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
    if (s.empty()) continue;
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
