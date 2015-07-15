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
    msg << __func__ << ": ERROR: File " << filename << " does not exist";
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
    case 5: character.SetIsDead(); break;
    case 6: DoHasItemChapter(s,chapter_number,character); break;
    case 7: ParseFight(s,chapter_number,character,auto_play); break;
    case 8: ParseChangeStatusAskOption(s,chapter_number,character,auto_play); break;
    case 9: ParseShop(s,chapter_number,character,auto_play); break;
    case 10: ParseFightWithTwoMonsters(s,chapter_number,character,auto_play); break;
    case 11: DoGameWon(); break;
    case 12: ParseFightWithRandomMonster(s,chapter_number,character,auto_play); break;
    case 13: ParsePawnShop(s,chapter_number,character,auto_play); break;
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
  /*
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
  */
  //s << std::noskipws;
  //const std::string at{ReadString(s)};
  //Parse(s,'@');
  //assert(at == "@");
  //New chapter
  /*
  int new_chapter = -1;
  {
    s >> new_chapter;
    assert(new_chapter > -1);
  }
  */
  /*
  assert(names.size() == staminas.size());
  assert(names.size() == dexterities.size());
  std::vector<Monster> monsters;
  for (int i=0; i!=n_monsters; ++i)
  {
    monsters.push_back(
      Monster(names[i],dexterities[i],staminas[i])
    );
  }
  */
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

void DoFight(
  std::vector<Monster> monsters,
  Character& character,
  const bool auto_play
)
{
  for (auto monster: monsters)
  {
    DoFight(monster,character,auto_play);
    if (character.IsDead()) return;
  }
}

void DoFightTwoMonsters(
  std::vector<Monster> monsters,
  Character& character,
  const bool auto_play
)
{
  const bool verbose{false};
  for (int i=0; ; ++i)
  {
    if (verbose) { std::clog << "Fight round " << i << std::endl; }
    if (character.IsDead())
    {
      if (verbose) { std::clog << "You got killed" << std::endl; }
      return;
    }
    if (monsters[0].IsDead())
    {
      if (verbose) { std::clog << "First monster killed" << std::endl; }
      break;
    }
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
        monsters[0].ChangeStamina(-2);
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

  std::cout << "You defeated the monster." << std::endl;

  //Fight the remaining monster normally
  DoFight(monsters[1],character,auto_play);
  if (character.IsDead()) return;
}


void DoFight(
  Monster monster,
  Character& character,
  const bool auto_play
)
{
  while (1)
  {
    if (character.IsDead()) break;
    if (monster.IsDead()) break;

    std::cout
      << "You " << character.GetDexterity() << " "
      << character.GetStamina() << "/"
      << character.GetInitialStamina()
      << std::endl
      << monster.GetName() << " "
      << monster.GetDexterity() << " "
      << monster.GetStamina() << "/"
      << monster.GetInitialStamina()
      << std::endl;
    ;

    const int monster_attack = (std::rand() >> 4) % 6;
    const int player_attack = (std::rand() >> 4) % 12;
    if (player_attack > monster_attack)
    {
      std::cout << "You hit the monster." << std::endl;
      const int damage = 2;
      if (!auto_play)
      {
        //std::cout
        //  << "Do you want to use luck? [1] Yes [2] No\n"
        //  << std::endl
        //;
        //assert(!"TODO");
        monster.ChangeStamina(-damage);
      }
      else
      {
        monster.ChangeStamina(-damage);
      }
    }
    else if (player_attack < monster_attack)
    {
      std::cout << "You were hit by the monster." << std::endl;
      const int damage{monster.GetAttackStrength()};
      if (!auto_play)
      {
        //std::cout << "Do you want to use luck?" << std::endl;
        //assert(!"TODO");
        character.ChangeStamina(-damage);
      }
      else
      {
        character.ChangeStamina(-damage);
      }
    }
    else
    {
      std::cout << "No damage was dealt." << std::endl;
    }
  }

  if (character.IsDead())
  {
    std::cout << "The monster defeated you.\n";
  }
  else
  {
    std::cout << "You defeated the monster." << std::endl;
  }
}

void DoFightWithTime(std::stringstream& s, int& chapter, Character& character, const bool auto_play)
{
  const bool verbose{false};
  Parse(s,'@');

  //Name monster
  std::string name;
  {
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
    s >> monster_dexterity;
    assert(monster_dexterity != -1);
  }
  if (verbose) { std::cout << "Monster dexterity: " << monster_dexterity << std::endl; }
  //Condition monster
  int monster_stamina = -1;
  {
    Parse(s,'@');
    s >> monster_stamina;
    assert(monster_stamina != -1);
  }
  if (verbose) { std::cout << "Monster stamina: " << monster_stamina << std::endl; }
  //Number of rounds
  int number_of_rounds = -1;
  {
    Parse(s,'@');
    s >> number_of_rounds;
    assert(number_of_rounds > -1);
  }
  if (verbose) { std::cout << "Number of rounds: " << number_of_rounds << std::endl; }
  //New chapter after time limit
  int new_chapter_after_time_limit = -1;
  {
    Parse(s,'@');
    s >> new_chapter_after_time_limit;
    assert(new_chapter_after_time_limit > -1);
  }
  if (verbose) { std::cout << "New chapter after time limit: " << new_chapter_after_time_limit << std::endl; }
  //New chapter after time limit
  int new_chapter_within_time_limit = -1;
  {
    Parse(s,'@');
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
        << "\n";
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

void DoGameOver()
{
  std::cout
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
      int change_gold = -123;
      s >> change_gold;
      assert(change_gold != -123);
      if (has_dex) { character.ChangeGold(change_gold); }
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

void Parse(std::stringstream& s, const char expected_char)
{
  char c = ReadChar(s);
  assert(c == expected_char);
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
    if (status == '0') return;
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
      msg << __func__ << ": ERROR: Chapter " << chapter << "\'s option " << option
        << " brings player to the same chapter";
      throw std::runtime_error(msg.str());
    }
  }
  assert(!options.empty());
  DoNormalChapter(options,chapter,auto_play);
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
  const Character& character,
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
      msg << __func__ << ": ERROR: Chapter " << chapter << "\'s option " << option
        << " brings player to the same chapter";
      throw std::runtime_error(msg.str());
    }
  }
  assert(!options.empty());
  DoNormalChapter(options,chapter,auto_play);
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

  /*
  //Parse the items
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
  */
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

  /*
  //Parse the items
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
  */
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

Item ReadItem(std::stringstream& s)
{
  int number = -1;
  s >> number;
  assert(number > -1);
  const Item item = static_cast<Item>(number);
  return item;
}

std::string ReadString(std::stringstream& s)
{
  std::string str = "";
  s >> str;
  return str;
}
