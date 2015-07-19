#include "dochapter.h"

#include <algorithm>
#include <cassert>
#include <string>
#include <iterator>
#include <iostream>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include "chapter.h"
#include "character.h"
#include "helper.h"
#include "monster.h"

void DoChangeStatusChapter(std::stringstream& s, int& chapter, Character& character)
{
  const bool verbose{false};
  if (verbose) { std::clog << "CHAPTER " << chapter << std::endl; }
  ParseChangeStatus(s,character);
  //Parse next chapter
  {
    Parse(s,'@');
    int next_chapter = -1;
    s >> next_chapter;
    assert(next_chapter > -1);
    chapter = next_chapter;
  }
}



void DoChapter(
  int& chapter_number,
  Character& character,
  const Language language,
  const bool auto_play
)
{
  const std::string filename{
    (language == Language::Dutch ? "../Bestanden/" : "../Files/")
    + std::to_string(chapter_number) + ".txt"
  };
  if (!IsRegularFile(filename))
  {
    std::stringstream msg;
    //msg << __func__ << ": ERROR: File " << filename << " does not exist";
    throw std::runtime_error(msg.str());
  }
  try
  {
    const Chapter chapter(filename);
    chapter.Do(character,auto_play);
    chapter_number = character.GetCurrentChapter();
    return;
  }
  catch (...)
  {
    //No problem, use old interface
  }
  assert(language == Language::Dutch); //Obsolete in English
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
      if (c == '[') { std::cout << '\n'; pos = 0; }
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
  int chapter_type = -1;
  s >> chapter_type;

  switch (chapter_type)
  {
    case 0: ParseNormalChapter(s,chapter_number,character,auto_play); break;
    case 1: DoTestYourLuckChapter(s,chapter_number,character); break;
    case 2: DoTestYourDexterityChapter(s,chapter_number,character); break;
    case 3: DoChangeStatusChapter(s,chapter_number,character); break;
    case 4: DoFightWithTime(s,chapter_number,character,auto_play); break;
    case 5: assert(!"Obsolete"); character.SetIsDead(); break;
    case 6: DoHasItemChapter(s,chapter_number,character); break;
    case 7: assert(!"Obsolete"); ParseFight(s,chapter_number,character,auto_play); break;
    case 8: ParseChangeStatusAskOption(s,chapter_number,character,auto_play); break;
    case 9: ParseShop(s,chapter_number,character,auto_play); break;
    case 10: assert(!"Obsolete"); ParseFightWithTwoMonsters(s,chapter_number,character,auto_play); break;
    case 11: assert(!"Obsolete"); DoGameWon(); break;
    case 12: ParseFightWithRandomMonster(s,chapter_number,character,auto_play); break;
    case 13: ParsePawnShop(s,chapter_number,character,auto_play); break;
    case 14: assert(!"Obsolete"); break;
    case 15: assert(!"Obsolete");  break;
    case 16: assert(!"Obsolete");  break;
    default:
    {
      std::stringstream msg;
      msg << __func__ << ": ERROR: Chapter " << chapter_number << " does not have a (supported) code, "
        << "chapter_type: " << chapter_type
        << std::endl; return;
      throw std::runtime_error(msg.str());
    }
  }
}

void DoInventory(Character& character, const bool auto_play)
{
  std::stringstream s;
  s
    << "Dexterity:\n"
    << " * Base: " << character.GetDexterityBase() << "/" << character.GetInitialDexterity() << '\n'
  ;

  //Shield
  if (character.HasItem(Item::shield_with_tower_crest))
  {
    s << " * " << ToStr(Item::shield_with_tower_crest) << ": -1 (equipped, cursed)\n";
    if (character.HasItem(Item::shield_with_unicorn_crest)) { s << " * " << ToStr(Item::shield_with_unicorn_crest) << ": +3\n"; }
    if (character.HasItem(Item::magnificent_shield)) { s << " * " << ToStr(Item::magnificent_shield) << ": +2\n"; }
    if (character.HasItem(Item::shield)) { s << " * " << ToStr(Item::shield) << ": +1\n"; }
  }
  else if (character.HasItem(Item::shield_with_unicorn_crest))
  {
    s << " * " << ToStr(Item::shield_with_unicorn_crest) << ": +3 (equipped)\n";
    if (character.HasItem(Item::magnificent_shield)) { s << " * " << ToStr(Item::magnificent_shield) << ": +2\n"; }
    if (character.HasItem(Item::shield)) { s << " * " << ToStr(Item::shield) << ": +1\n"; }
  }
  else if (character.HasItem(Item::magnificent_shield))
  {
    if (character.HasItem(Item::magnificent_shield)) { s << " * " << ToStr(Item::magnificent_shield) << ": +2 (equipped)\n"; }
    if (character.HasItem(Item::shield)) { s << " * " << ToStr(Item::shield) << ": +1\n"; }
  }
  else if (character.HasItem(Item::shield))
  {
    if (character.HasItem(Item::shield)) { s << " * " << ToStr(Item::shield) << ": +1 (equipped)\n"; }
  }

  if (character.HasItem(Item::carralifs_sword))
  {
    s << " * " << ToStr(Item::carralifs_sword) << ": +2 (equipped) \n";
    if (character.HasItem(Item::ordinary_sword)) { s << " * " << ToStr(Item::ordinary_sword) << ": +1\n"; }
  }
  else if (character.HasItem(Item::ordinary_sword))
  {
    if (character.HasItem(Item::ordinary_sword)) { s << " * " << ToStr(Item::ordinary_sword) << ": +1 (equipped)\n"; }
  }

  if (character.HasItem(Item::magic_elven_boots)) { s << " * " << ToStr(Item::magic_elven_boots) << ": +1\n"; }
  if (character.HasItem(Item::chainmail_coat)) { s << " * " << ToStr(Item::chainmail_coat) << ": +2\n"; }
  s
    << " * Total: " << character.GetDexterity() << "/" << character.GetInitialDexterity() << '\n'
    << "Stamina: " << character.GetStamina() << "/" << character.GetInitialStamina() << '\n'
    << "Luck:\n"
    << " * Base: " << character.GetLuckBase() << "/" << character.GetInitialLuck() << '\n'
  ;

  if (character.HasItem(Item::golden_scorpion_brooch)) { s << " * " << ToStr(Item::golden_scorpion_brooch) << ": +2\n"; }
  //Note: the copper brooch decreases luck with 1, but this is not shown on purpose
  //I cannot avoid that it will be easy to see that base luck and total luck don't match
  s
    << " * Total: " << character.GetLuck() << "/" << character.GetInitialDexterity() << '\n'
    << "Gold pieces: " << character.GetGold() << '\n'
    << "Provisions: " << character.GetProvisions() << '\n'
    << '\n'
  ;

  if (character.GetArrows() != 0)
  {
    s << "You got " << character.GetArrows() << " sticking in your body\n";
  }
  for (const Item item: character.GetItems())
  {
    s << ToStr(item) << '\n';
  }
  ShowText(s.str(),auto_play);
}

void DoPlayBall(Character& character, const bool auto_play)
{
  /*
Roll one die alternately for yourself and for the
bare-chested man, to represent the cannon-fcuH pas-
sing between you. Repeat this process until a 1 is
thrown, in which case the cannon-tall has
been dropped and the loser must pay the winner 5
Gold Pieces.
  */

  {
    std::stringstream s;
    s << "The bare-chested man throws the ball to you.\n";
    ShowText(s.str(),auto_play);
  }
  if (!auto_play) { Wait(0.5); }

  while (1)
  {
    const int dice_you{1 + ((std::rand() >> 4) % 6)};
    if (dice_you == 1)
    {
      std::stringstream s;
      s << "You drop the ball and pay the bare-chested man 5 gold pieces.\n";
      ShowText(s.str(),auto_play);
      if (character.GetGold() >= 5)
      {
        character.ChangeGold(-5);
      }
      else
      {
        std::clog << "WARNING: should not have got here, due to lack of gold\n";
        character.ChangeGold(-character.GetGold());
      }
      return;
    }
    {
      std::stringstream s;
      s << "You catch the ball and throw it to the bare-chested man.\n";
      ShowText(s.str(),auto_play);
    }
    if (!auto_play) { Wait(0.5); }
    const int dice_man{1 + ((std::rand() >> 4) % 6)};
    if (dice_man == 1)
    {
      std::stringstream s;
      s << "The bare-chested man drops the ball and gives you 5 gold pieces.\n";
      ShowText(s.str(),auto_play);
      character.ChangeGold(5);
      return;
    }
    {
      std::stringstream s;
      s << "The bare-chested man catched the ball and throws it back to you.\n";
      ShowText(s.str(),auto_play);
    }
    if (!auto_play) { Wait(0.5); }
  }
}

void DoPlayDice(Character& character, const bool auto_play)
{
  /*

The stakes are 2 gold
pieces. Each person will stake 2 gold pieces and roll
two dice. The person rolling the highest number
collects the 8 gold pieces. To play, roll two dice
three limes for the three Dwarfs and make a note of
each total. Next, roll two dice for yourself. If your
own total is higher than each of the other three
totals, you win 6 gold pieces from the Dwarfs. If the
total is' less than any of the other three totals, you
lose 2 Gold Pieces. You may play four times if you
wish to

  */
  if (character.GetGold() < 2)
  {
    std::stringstream s;
    s << "You cannot afford to play this game.";
    ShowText(s.str(),auto_play);
    return;
  }

  while (1)
  {
    {
      std::stringstream s;
      s
        << "Do you want to play?\n"
        << "[1] Yes\n"
        << "[2] No\n"
      ;
      ShowText(s.str(),auto_play);
    }
    std::string s;
    if (!auto_play)
    {
      std::getline(std::cin,s);
    }
    else
    {
      switch ((std::rand() >> 4) % 3)
      {
        case 0: s = "1"; break;
        case 1: s = "2"; break;
        case 2: s = "x"; break;
      }
    }
    if (s == "2") { return; }
    if (s == "1") { break; }
    {
      std::stringstream s;
      s
        << "Please enter either '1' or '2'.\n"
      ;
      ShowText(s.str(),auto_play);
    }
  }


  const int n_rounds{4};
  for (int round = 0; round != 4; ++round)
  {

    {
      std::stringstream s;
      s << "Round #" << (round + 1) << "/" << n_rounds << '\n';
      ShowText(s.str(),auto_play);
    }
    //You
    const int dice_you = 1 + ((std::rand() >> 4) % 6);
    const int dice_1 = 1 + ((std::rand() >> 4) % 6);
    const int dice_2 = 1 + ((std::rand() >> 4) % 6);
    const int dice_3 = 1 + ((std::rand() >> 4) % 6);
    {
      std::stringstream s;
      s << "You throw: " << dice_you << '\n';
      ShowText(s.str(),auto_play);
      if (!auto_play) { Wait(0.5); }
    }
    {
      std::stringstream s;
      s << "Dwarf 1 throws: " << dice_1 << '\n';
      ShowText(s.str(),auto_play);
      if (!auto_play) { Wait(0.5); }
    }
    {
      std::stringstream s;
      s << "Dwarf 2 throws: " << dice_2 << '\n';
      ShowText(s.str(),auto_play);
      if (!auto_play) { Wait(0.5); }
    }
    {
      std::stringstream s;
      s << "Dwarf 3 throws: " << dice_3 << '\n';
      ShowText(s.str(),auto_play);
      if (!auto_play) { Wait(0.5); }
    }
    if (dice_you > dice_1 && dice_you > dice_2 && dice_you > dice_3)
    {
      std::stringstream s;
      s << "You won! You collect your 6 gold pieces from the three dwarves.\n";
      ShowText(s.str(),auto_play);
      character.ChangeGold(6);
      if (!auto_play) { Wait(0.5); }
    }
    else if (dice_you < dice_1 || dice_you < dice_2 || dice_you < dice_3)
    {
      std::stringstream s;
      s << "You Lost! You lose 2 gold pieces to the three dwarves.\n";
      ShowText(s.str(),auto_play);
      character.ChangeGold(-2);
      if (!auto_play) { Wait(0.5); }
    }
    else
    {
      std::stringstream s;
      s << "A draw. You keep your gold\n";
      ShowText(s.str(),auto_play);
      if (!auto_play) { Wait(0.5); }
    }

    if (character.GetGold() < 2)
    {
      std::stringstream s;
      s << "You cannot afford to play another round.";
      ShowText(s.str(),auto_play);
      break;
    }

    if (round == 3) break;

    //Play again?
    while (1)
    {
      {
        std::stringstream s;
        s
          << "Do you want to play another round?\n"
          << "[1] Yes\n"
          << "[2] No\n"
        ;
        ShowText(s.str(),auto_play);
      }
      std::string s;
      if (!auto_play)
      {
        std::getline(std::cin,s);
      }
      else
      {
        switch ((std::rand() >> 4) % 3)
        {
          case 0: s = "1"; break;
          case 1: s = "2"; break;
          case 2: s = "x"; break;
        }
      }
      if (s == "2") { return; }
      if (s == "1") { break; }
      {
        std::stringstream s;
        s
          << "Please enter either '1' or '2'.\n"
        ;
        ShowText(s.str(),auto_play);
      }
    }
  }
}

void DoPlayPill(Character& character, const bool auto_play)
{
  const int dice{1 + ((std::rand() >> 4) % 6)};
  if (dice == 1)
  {
    std::stringstream s;
    s << "You die quickly from the poisoned pill and your adventure ends here.\n";
    ShowText(s.str(),auto_play);
    character.SetIsDead();
  }
}

void ParseFight(std::stringstream& s, int& chapter, Character& character, const bool auto_play)
{
  const bool verbose{true};
  s << std::skipws; //Obligatory
  int next_chapter = 0;
  std::vector<Monster> monsters;
  while (1)
  {
    const std::string str{ReadString(s)};
    if (str == "Monster")
    {
      const std::string name{ReadString(s)};
      const int dexterity{ReadInt(s)};
      assert(dexterity > 0);
      const int condition{ReadInt(s)};
      assert(condition > 0);
      const int attack_strength{ReadInt(s)};
      assert(attack_strength > 0);
      const Monster monster(name,dexterity,condition,attack_strength);
      if (verbose) { std::clog << "Parsed monster: " << monster << std::endl; }
      monsters.push_back(monster);
    }
    else if (str == "Next_chapter")
    {
      next_chapter = ReadInt(s);
      if (verbose) { std::clog << "Parsed next_chapter: " << next_chapter << std::endl; }
    }
    else
    {
      break;
    }

  }
  if (monsters.empty())
  {
    std::cerr << "No monsters in chapter " << chapter << std::endl;
  }
  if (!next_chapter)
  {
    std::cerr << "No Next_chapter in chapter " << chapter << std::endl;
  }
  assert(!monsters.empty());
  assert(next_chapter != 0);
  DoFight(monsters,character,auto_play);
  //assert(chapter != 0); //Game over does not go here
  chapter = next_chapter;
}

void ParseFightWithRandomMonster(std::stringstream& s, int& chapter, Character& character, const bool auto_play)
{
  //const bool verbose{false};
  Parse(s,'@');
  const int chapter_non_apeman{ReadInt(s)};
  Parse(s,'@');
  const int chapter_apeman{ReadInt(s)};

  const int which_monster = 1 + (std::rand() % 6);
  Monster * monster = nullptr;
  switch (which_monster)
  {
    case 1: monster = new Monster("Orc",3,3); break;
    case 2: monster = new Monster("Giant snake",4,6); break;
    case 3: monster = new Monster("Wolf",5,5); break;
    case 4: monster = new Monster("Pygmy",4,4); break;
    case 5: monster = new Monster("Ape man",7,6); break;
    case 6: monster = new Monster("Cave troll",8,7); break;
  }
  DoFight(*monster,character,auto_play);
  delete monster;
  chapter = which_monster == 5 ? chapter_apeman : chapter_non_apeman;
}

void ParseFightWithTwoMonsters(std::stringstream& s, int& chapter, Character& character, const bool auto_play)
{
  const bool verbose{false};
  Parse(s,'@');
  //Name monsters
  std::vector<std::string> names;
  {
    std::string name;
    while (1)
    {
      char c = '*';
      s >> c;
      assert(c != '*');
      if (c == ',') { names.push_back(name); name = ""; continue; }
      if (c == '@') { names.push_back(name); break; }
      name += c;
    }
  }

  const int n_monsters{static_cast<int>(names.size())};

  if (verbose)
  {
    std::clog << "# monsters: " << n_monsters << std::endl;
    std::copy(std::begin(names),std::end(names),std::ostream_iterator<std::string>(std::clog," "));
    std::clog << std::endl;
  }


  //Skill monsters
  std::vector<int> dexterities;

  for (int i=0; i!=n_monsters; ++i)
  {
    int dexterity = -1;
    s >> dexterity;
    assert(dexterity != -1);
    dexterities.push_back(dexterity);
    if (i < n_monsters - 1)
    {
      Parse(s,',');
    }
  }

  if (verbose)
  {
    std::copy(std::begin(dexterities),std::end(dexterities),std::ostream_iterator<int>(std::clog," "));
    std::clog << std::endl;
  }

  //At
  Parse(s,'@');
  //Condition monster
  std::vector<int> staminas;
  for (int i=0; i!=n_monsters; ++i)
  {
    int stamina = -1;
    s >> stamina;
    assert(stamina != -1);
    staminas.push_back(stamina);
    if (i < n_monsters - 1)
    {
      Parse(s,',');
    }
  }
  Parse(s,'@');
  //New chapter
  int new_chapter = -1;
  {
    s >> new_chapter;
    assert(new_chapter > -1);
  }
  assert(names.size() == staminas.size());
  assert(names.size() == dexterities.size());
  std::vector<Monster> monsters;
  for (int i=0; i!=n_monsters; ++i)
  {
    monsters.push_back(
      Monster(names[i],dexterities[i],staminas[i])
    );
  }

  DoFightTwoMonsters(monsters,character,auto_play);
  assert(chapter != 0); //Game over does not go here
  chapter = new_chapter;
}
























void DoGameOver()
{
  std::cout
    << '\n'
    << "*************\n"
    << "*           *\n"
    << "* GAME OVER *\n"
    << "*           *\n"
    << "*************\n"
  ;
}

void DoGameWon()
{
  std::cout
    << '\n'
    << "*************\n"
    << "*           *\n"
    << "* GAME WON! *\n"
    << "*           *\n"
    << "*************\n"
  ;
}

void DoHasItemChapter(std::stringstream& s, int& chapter, Character& character)
{
  const bool verbose{true};
  if (verbose) { std::clog << "CHAPTER " << chapter << std::endl; }
  std::vector<int> item_numbers;
  int gold_pieces = 0;
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
      const char what{ReadChar(s)};
      assert(what == 'G' || what == 'I');
      switch (what)
      {
        case 'G':
        {
          gold_pieces = ReadInt(s);
          if (verbose) { std::clog << "Needed gold: " << static_cast<Item>(gold_pieces) << std::endl; }
        }
        break;
        case 'I':
        {
          const int item_number{ReadInt(s)};
          item_numbers.push_back(item_number);
          if (verbose) { std::clog << "Needed item: " << static_cast<Item>(item_number) << std::endl; }
        }
        break;
      }
      char comma_or_at = '*';
      s >> comma_or_at;
      assert(comma_or_at != '*');
      if (comma_or_at != ',') break;
    }
  }

  int chapter_if_not_have = -1;
  //Parse chapter if not have
  {
    Parse(s,'@');
    Parse(s,'0');
    Parse(s,':');
    s >> chapter_if_not_have;
    if (verbose) { std::clog << "chapter_if_not_have: " << chapter_if_not_have << std::endl; }
  }
  assert(chapter_if_not_have != -1);
  int chapter_if_have = -1;
  //Parse chapter if have
  {
    Parse(s,'@');
    Parse(s,'1');
    Parse(s,':');
    s >> chapter_if_have;
    if (verbose) { std::clog << "chapter_if_have: " << chapter_if_have << std::endl; }
  }
  assert(chapter_if_have != -1);
  //Has the items needed?
  for (const int item_number: item_numbers)
  {
    const Item item = static_cast<Item>(item_number);
    if (!character.HasItem(item))
    {
      chapter = chapter_if_not_have;
      return;
    }
  }
  //Has the gold needed?
  if (character.GetGold() < gold_pieces)
  {
    chapter = chapter_if_not_have;
    return;
  }
  //Has everything needed
  chapter = chapter_if_have;
}

void DoNormalChapter(
  std::vector<std::pair<char,int>> options,
  int& chapter,
  const bool auto_play)
{
  //Chose an options
  if (auto_play)
  {
    std::random_shuffle(std::begin(options),std::end(options));
    std::cout << "AUTOPLAY: chose option " << options[0].first << std::endl;
    chapter = options[0].second;
    return;
  }
  //Only one option
  if (options.size() == 1)
  {
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

void DoPawnShop(
  std::vector<std::pair<Item,int>> items,
  const std::string& exit_text,
  Character& character,
  const bool auto_play
)
{
  while (1)
  {
    if (items.empty())
    {
      std::cout << "There are no more items to sell.\n";
      break;
    }
    std::cout << "[0] Leave shop\n";
    const int n_items{static_cast<int>(items.size())};
    for (int i=0; i!=n_items; ++i)
    {
      std::cout << '[' << (i+1) << "] Sell "
        << ToStr(items[i].first) << " for "
        << items[i].second << " gold pieces\n"
      ;
    }
    if (auto_play)
    {
      for (int i=0; i < static_cast<int>(items.size()); ++i)
      {
        if (items[i].second <= character.GetGold())
        {
          std::cout << "You sold " << ToStr(items[i].first) << std::endl;
          character.AddItem(items[i].first);
          character.ChangeGold(items[i].second);
          std::swap(items[i],items.back());
          items.pop_back();
          --i;
        }
      }
      break;
    }
    assert(!auto_play);

    //Shop
    std::string s;
    std::getline(std::cin,s);
    if (s.empty()) continue;
    try
    {
      boost::lexical_cast<int>(s);
    }
    catch (boost::bad_lexical_cast&)
    {
      std::cout << "Invalid command\n";
      continue;
    }
    const int command = boost::lexical_cast<int>(s);
    if (command == 0) break;
    const int i = command - 1;
    if (i < 0 || i >= static_cast<int>(items.size()))
    {
      std::cout << "Invalid number, choose zero to leave the shop or an item number to buy it.\n";
      continue;
    }
    assert(i >= 0);
    assert(i < static_cast<int>(items.size()));
    std::cout << "You sold " << ToStr(items[i].first) << std::endl;
    character.RemoveItem(items[i].first);
    character.ChangeGold(items[i].second);
    std::swap(items[i],items.back());
    items.pop_back();
  }
  std::cout << exit_text << std::endl;
}

void DoShop(
  std::vector<std::pair<Item,int>> items,
  const std::string& exit_text,
  Character& character,
  const bool auto_play
)
{
  while (1)
  {
    if (items.empty())
    {
      std::cout << "There are no more items to buy.\n";
      break;
    }
    std::cout << "[0] Leave shop\n";
    const int n_items{static_cast<int>(items.size())};
    bool can_buy{false};
    for (int i=0; i!=n_items; ++i)
    {
      std::cout << '[' << (i+1) << "] Buy "
        << ToStr(items[i].first) << " for "
        << items[i].second << " gold pieces\n"
      ;
      if (items[i].second <= character.GetGold()) { can_buy = true; }
    }
    if (!can_buy)
    {
      std::cout << "You do not have enough money to buy anything.\n";
      break;
    }
    if (auto_play)
    {
      for (int i=0; i < static_cast<int>(items.size()); ++i)
      {
        if (items[i].second <= character.GetGold())
        {
          std::cout << "You bough " << ToStr(items[i].first) << std::endl;
          character.AddItem(items[i].first);
          character.ChangeGold(-items[i].second);
          std::swap(items[i],items.back());
          items.pop_back();
          --i;
        }
      }
      break;
    }
    assert(!auto_play);

    //Shop
    std::string s;
    std::getline(std::cin,s);
    if (s.empty()) continue;
    try
    {
      boost::lexical_cast<int>(s);
    }
    catch (boost::bad_lexical_cast&)
    {
      std::cout << "Invalid command\n";
      continue;
    }
    const int command = boost::lexical_cast<int>(s);
    if (command == 0) break;
    const int i = command - 1;
    if (i < 0 || i >= static_cast<int>(items.size()))
    {
      std::cout << "Invalid number, choose zero to leave the shop or an item number to buy it.\n";
      continue;
    }
    assert(i >= 0);
    assert(i < static_cast<int>(items.size()));
    if (character.GetGold() < items[i].second)
    {
      std::cout << "Cannot buy this item: not enough gold\n";
      continue;
    }
    std::cout << "You bough " << ToStr(items[i].first) << std::endl;
    character.AddItem(items[i].first);
    character.ChangeGold(-items[i].second);
    std::swap(items[i],items.back());
    items.pop_back();
  }
  std::cout << exit_text << std::endl;
}

void DoTestYourDexterityChapter(std::stringstream& s, int& chapter, Character& character)
{
  const bool has_dex{character.TestDexterity()};

  //Parse no dexterity
  int no_dex_chapter = -1;
  std::string no_dex_text;
  {
    Parse(s,'@');
    Parse(s,'0');
    Parse(s,':');
    while (!s.eof())
    {
      char c;
      s >> c;
      if (c == '@') break;
      no_dex_text += c;
    }
    const char status = ReadChar(s);
    if (status == 'G')
    {
      const int change_gold = ReadInt(s);
      if (has_dex)
      {
        //Allow for having no money
        if (character.GetGold() + change_gold < 0)
        {
          std::clog << "WARNING: character could not afford this" << std::endl;
          character.ChangeGold(-character.GetGold());
          assert(character.GetGold() == 0);
        }
        else
        {
          character.ChangeGold(change_gold);
        }
      }
      Parse(s,',');
    }
    else
    {
      s.putback(status);
    }
    s >> no_dex_chapter;
    assert(no_dex_chapter > -1);
    assert(!no_dex_text.empty());
  }

  //Parse dex
  int dex_chapter = -1;
  std::string dex_text;
  {
    Parse(s,'@');
    Parse(s,'1');
    Parse(s,':');
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
  const bool verbose{false};

  //Luck is already known
  const bool has_luck{character.TestLuck()};

  //Parse no luck
  int no_luck_chapter = -1;
  std::string no_luck_text;
  {
    Parse(s,'@');
    Parse(s,'0');
    Parse(s,':');
    while (!s.eof())
    {
      char c;
      s >> c;
      if (c == '@') break;
      no_luck_text += c;
    }
    char status = '*';
    s >> status;
    assert(status != '*');
    if (status == 'S')
    {
      const int change = ReadInt(s);
      if (!has_luck) { character.ChangeStamina(change); }
      Parse(s,',');
    }
    else
    {
      if (verbose) { std::cout << "Put back: " << status << std::endl; }
      s.putback(status);
    }
    s >> no_luck_chapter;
    assert(no_luck_chapter > -1);
    assert(!no_luck_text.empty());
  }

  //Parse luck
  int luck_chapter = -1;
  std::string luck_text;
  {
    Parse(s,'@');
    Parse(s,'1');
    Parse(s,':');
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


void ParseChangeStatus(std::stringstream& s, Character& character)
{
  const bool verbose{false};

  Parse(s,'@');
  while (1)
  {
    assert(!s.eof());
    char status = '*';
    s >> status;
    assert(status != '*');
    if (verbose) { std::clog << "status: " << status << std::endl; }
    if (status == '0')
    {
      assert(!"Should not get here");
      return;
    }
    if (status == 'I')
    {
      const Item item = ReadItem(s);
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
          const Item conditional_item = ReadItem(s);
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
      if (change == 997) { change = 3 * (1 + std::rand() % 6); }
      if (change == 999) { change = 1 + std::rand() % 6; }
      switch (status)
      {
        case 'D':
          if (verbose) { std::clog << "Change dexterity by " << change << std::endl; }
          character.ChangeDexterity(change);
        break;
        case 'G':
          if (change == 998) { change = -character.GetGold(); }
          if (verbose) { std::clog << "Change gold by " << change << std::endl; }
          character.ChangeGold(change);
        break;
        case 'P':
          if (verbose) { std::clog << "Change provisions by " << change << std::endl; }
          character.ChangeProvisions(change);
        break;
        case 'S':
          if (verbose) { std::clog << "Change stamina by " << change << std::endl; }
          character.ChangeStamina(change);
        break;
        case 'L':
          if (verbose) { std::clog << "Change luck by " << change << std::endl; }
          character.ChangeLuck(change);
        break;
        case '0': break;
        default:
          std::cerr << status << std::endl;
          assert(!"Should not get here");
      }
    }
    char comma_or_at = '*';
    s >> comma_or_at;
    assert(!s.eof());
    while (!s.eof() && (comma_or_at == '\n' || comma_or_at == ' ')) { s >> comma_or_at; }
    if (comma_or_at == '@') { s.putback('@'); break; }
  }
}

void ParseChangeStatusAskOption(
  std::stringstream& s,
  int& chapter,
  Character& character,
  const bool auto_play)
{
  assert(1==2);
  //const bool verbose{true};

  //Parse change status
  ParseChangeStatus(s,character);

  //Parse the options
  std::vector<std::pair<char,int>> options; //input, new chapter

  while (1)
  {
    char at;
    assert(!s.eof());
    s >> at;
    assert(!s.eof());
    while (!s.eof() && (at == '\t' || at == '\n' || at == ' ')) { s >> at; }
    if(s.eof()) break;
    assert(at == '@');
    assert(!s.eof());
    char option;
    assert(!s.eof());
    s >> option;
    char colon_or_question_mark;
    assert(!s.eof());
    s >> colon_or_question_mark;
    assert(colon_or_question_mark == ':' || colon_or_question_mark == '?');
    bool can_choose{true};
    if (colon_or_question_mark == '?')
    {
      Parse(s,'I');
      const Item item{ReadItem(s)};
      if (!character.HasItem(item)) { can_choose = false; }
      Parse(s,':');
    }
    int chapter_to_go;
    assert(!s.eof());
    s >> chapter_to_go;
    if (can_choose)
    {
      options.push_back(std::make_pair(option,chapter_to_go));
    }
    if (chapter == chapter_to_go)
    {
      std::stringstream msg;
      msg << __func__ << ": WARNING: Chapter " << chapter << "\'s option " << option
        << " brings player to the same chapter";
      throw std::runtime_error(msg.str());
    }
  }
  assert(!options.empty());
  DoNormalChapter(options,chapter,auto_play);
  character.SetChapter(chapter);
}

Consequence ParseConsequence(std::stringstream &s)
{
  Consequence consequence;
  const std::string what{ReadString(s)};
  if (what == "add")
  {
    const std::string item_name{ReadString(s)};
    if (!IsItem(item_name))
    {
      std::cerr << "Unknown item: " << item_name << std::endl;
      assert(!"Should not get here");
    }
    const Item item{ToItem(item_name)};
    consequence.AddItemToAdd(item);
  }
  else if (what == "arrows" || what == "arrow")
  {
    const std::string value{ReadString(s)};
    if (value == "random[1-6]")
    {
      const int n_arrows{1 + ((std::rand() >> 4) % 6)};
      consequence.SetChangeArrows(n_arrows);
    }
    else if (value == "remove_all")
    {
      consequence.SetChangeArrows(-6);
    }
    else
    {
      assert(!"Should not get here");
    }
  }
  else if (what == "dexterity" || what == "dex")
  {
    const int change_dex{ReadInt(s)};
    consequence.SetChangeDexterity(change_dex);
  }
  else if (what == "gold")
  {
    const int change_gold{ReadInt(s)};
    consequence.SetChangeGold(change_gold);
  }
  else if (what == "luck")
  {
    const int change_luck{ReadInt(s)};
    consequence.SetChangeLuck(change_luck);
  }
  else if (what == "provision" || what == "provisions")
  {
    const int change_provisions{ReadInt(s)};
    consequence.SetChangeProvisions(change_provisions);
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
    consequence.AddItemToRemove(item);
  }
  else if (what == "stamina" || what == "sta")
  {
    const std::string value{ReadString(s)};
    if (IsInt(value))
    {
      consequence.SetChangeStamina(std::stoi(value));
    }
    else
    {
      assert(value == "random[1-6]");
      consequence.SetChangeStamina(1 + ((std::rand() >> 4) % 6));
    }
  }
  else
  {
    std::cerr << "Unknown what: " << what << std::endl;
    assert(!"Should not get here");
  }
  return consequence;
}
std::vector<std::pair<Item,int>> ParseItemWithPrices(std::stringstream& s)
{
  std::vector<std::pair<Item,int>> items;

  while (1)
  {
    Parse(s,'I');
    const Item item = ReadItem(s);
    Parse(s,'?');
    int price = -1;
    s >> price;
    assert(price != -1);
    items.push_back(std::make_pair(item,price));
    char comma_or_not = '*';
    s >> comma_or_not;
    assert(comma_or_not != '*');
    if (comma_or_not != ',') break;
  }
  return items;
}


void ParseNormalChapter(
  std::stringstream& s,
  int& chapter,
  Character& character,
  const bool auto_play)
{
  //Parse the options
  std::vector<std::pair<char,int>> options; //input, new chapter

  while (1)
  {
    char at;
    assert(!s.eof());
    s >> at;
    assert(!s.eof());
    while (!s.eof() && (at == '\t' || at == '\n' || at == ' ')) { s >> at; }
    if(s.eof()) break;
    assert(at == '@');
    assert(!s.eof());
    char option;
    assert(!s.eof());
    s >> option;
    char colon_or_question_mark;
    assert(!s.eof());
    s >> colon_or_question_mark;
    assert(colon_or_question_mark == ':' || colon_or_question_mark == '?');
    bool can_choose{true};
    if (colon_or_question_mark == '?')
    {
      const char what = ReadChar(s);
      assert(what == 'I' || what == 'G' || what == 'P');
      switch (what)
      {
        case 'G':
        {
          const int price{ReadInt(s)};
          if (character.GetGold() < price) { can_choose = false; }
        }
        break;
        case 'I':
        {
          const Item item{ReadItem(s)};
          if (!character.HasItem(item)) { can_choose = false; }
        }
        break;
        case 'P':
        {
          const int n_provisions{ReadInt(s)};
          if (character.GetProvisions() < n_provisions) { can_choose = false; }
        }
        break;
      }
      Parse(s,':');
    }
    int chapter_to_go;
    assert(!s.eof());
    s >> chapter_to_go;
    if (can_choose)
    {
      options.push_back(std::make_pair(option,chapter_to_go));
    }
    if (chapter == chapter_to_go)
    {
      std::stringstream msg;
      msg << __func__ << ": WARNING: Chapter " << chapter << "\'s option " << option
        << " brings player to the same chapter";
      throw std::runtime_error(msg.str());
    }
  }
  assert(!options.empty());
  DoNormalChapter(options,chapter,auto_play);
  character.SetChapter(chapter);
}

void ParsePawnShop(
  std::stringstream& s,
  int& chapter,
  Character& character,
  const bool auto_play)
{
  Parse(s,'@');

  const std::vector<std::pair<Item,int>> items{
    ParseItemWithPrices(s)
  };

  Parse(s,'@');
  int next_chapter = -1;
  s >> next_chapter;
  assert(next_chapter != -1);
  Parse(s,':');
  std::string exit_text;
  while (!s.eof())
  {
    char c;
    s >> c;
    exit_text += c;
  }
  DoPawnShop(items,exit_text,character,auto_play);
  chapter = next_chapter;
}

void ParseShop(
  std::stringstream& s,
  int& chapter,
  Character& character,
  const bool auto_play)
{
  Parse(s,'@');

  const std::vector<std::pair<Item,int>> items{
    ParseItemWithPrices(s)
  };

  Parse(s,'@');
  int next_chapter = -1;
  s >> next_chapter;
  assert(next_chapter != -1);
  Parse(s,':');
  std::string exit_text;
  while (!s.eof())
  {
    char c;
    s >> c;
    exit_text += c;
  }
  DoShop(items,exit_text,character,auto_play);
  chapter = next_chapter;
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
