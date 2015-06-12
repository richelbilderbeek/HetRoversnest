// git clone https://github.com/richelbilderbeek/HetRoversnest
#include "KrijgTekst.h"
#include "LaatZien.h"
#include "VerwerkCommando.h"

bool MoetGelukWordenGetest(const int hoofdstuk)
{
  //Hoofdstukken waarin het geluk moet worden getest
  static const int array[] = { 49 };
  const int n = sizeof(array) / sizeof(int);
  Serial.println(n);
  for (int i=0; i!=n; ++i)
  {
    if (array[i] == hoofdstuk) return true;  
  }
  return false;
}

int LeesCommando()
{
  //Wacht op input
  while (!Serial.available())
  {
    delay(100); 
  }
  //Verwerk input
  String s;

  int i = 0;
  while (Serial.available())
  {
    i = Serial.parseInt();
  }
  return i;
}


void setup()
{
  Serial.begin(9600);  
  ///Het hoofdstuk in het boek
  int hoofdstuk = 1;

  if (KrijgTekst(1) == '\0') Serial.println("ERRROR");
  
  while (1)
  {
    Serial.println("---------------");
    Serial.print("Hoofdstuk: "); Serial.println(hoofdstuk);
    Serial.println("---------------");

    //Laat de gewone tekst zien
    {
      const char * const tekst = KrijgTekst(hoofdstuk);
      LaatZien(tekst);
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

    
    const int commando = LeesCommando();
    VerwerkCommando(hoofdstuk,commando);
    Serial.print("commando: "); Serial.println(commando);
  }
}

void loop() {}
