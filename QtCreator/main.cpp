#include <iostream>

#include "../HetRoversnest/KrijgTekst.h"
#include "../HetRoversnest/VerwerkCommando.h"

bool MoetGelukWordenGetest(const int hoofdstuk)
{
  //Hoofdstukken waarin het geluk moet worden getest
  static const int array[] = { 49 };
  const int n = sizeof(array) / sizeof(int);
  for (int i=0; i!=n; ++i)
  {
    if (array[i] == hoofdstuk) return true;
  }
  return false;
}


int main()
{
  ///Het hoofdstuk in het boek
  int hoofdstuk = 1;

  while (1)
  {
    //Laat de gewone tekst zien
    {
      const char * const tekst = KrijgTekst(hoofdstuk);
      std::cout << tekst << std::endl;
    }

    //Kijk of het geluk moet worden getest
    if (MoetGelukWordenGetest(hoofdstuk))
    {
      //const bool geluk = TestGeluk();
      //const char * const tekst = KrijgTekstGeluk(hoofdstuk,geluk);
      //LaatZien(tekst);
      //hoofdstuk = ReageerOpGeluk(hoofdstuk,geluk);
      continue;
    }


    int commando;
    std::cin >> commando;
    VerwerkCommando(hoofdstuk,commando);
  }
}
