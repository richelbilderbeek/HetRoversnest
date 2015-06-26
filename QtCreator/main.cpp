#include <cassert>
#include <iostream>

#include "helper.h"


int main()
{
  int chapter = 1;

  while (1)
  {
    const std::string filename{"../" + std::to_string(chapter) + ".txt"};
    assert(IsRegularFile(filename));
    std::ifstream f(filename);

    File file = SD.open(filename.c_str(), FILE_READ);

    int pos = 0;
    while (file.available())
    {
      const char c = file.read();
      if (c == '\n') continue;
      Serial.print(c);
      ++pos;
      if (pos > 60 && c == ' ')
      {
        Serial.println();
        pos = 0;
      }
    }

    //Laat de gewone tekst zien
    {
      const char * const tekst = KrijgTekst(chapter);
      std::cout << tekst << std::endl;
    }

    //Kijk of het geluk moet worden getest
    if (MoetGelukWordenGetest(chapter))
    {
      //const bool geluk = TestGeluk();
      //const char * const tekst = KrijgTekstGeluk(hoofdstuk,geluk);
      //LaatZien(tekst);
      //hoofdstuk = ReageerOpGeluk(hoofdstuk,geluk);
      continue;
    }


    int commando;
    std::cin >> commando;
    VerwerkCommando(chapter,commando);
  }
}
