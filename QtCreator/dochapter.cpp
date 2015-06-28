#include "dochapter.h"

#include <algorithm>
#include <cassert>
#include <string>
#include <iterator>
#include <iostream>
#include <sstream>

#include <boost/lexical_cast.hpp>

void DoNormalChapter(std::stringstream& s, int& chapter)
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
  //Process command
  while (1)
  {
    std::string s;
    //std::cin >> s;
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
