#include <Arduino.h>
#include "KrijgTekst.h"

///Simpel
void PrintHoofdstuk1(const int hoofdstuk)
{
  Serial.print("Hoofdstuk: ");
  Serial.println(hoofdstuk);
  
  for (int paragraaf = 0; ; ++paragraaf)
  {
    Serial.print("Paragraaf: ");
    Serial.println(paragraaf);
    const String tekst = KrijgTekst(hoofdstuk,paragraaf);
    if (tekst == "") break; //Klaar
    Serial.println(tekst);
    delay(100);
  }
  Serial.println("Printhoofdstuk klaar");
}

void PrintHoofdstuk2(const int hoofdstuk)
{
  const int n_letters_per_regel = 80;
  
  for (int paragraaf = 0; ; ++paragraaf)
  {
    const String tekst = KrijgTekst(hoofdstuk,paragraaf);
    const int lengte = tekst.length();
    for (int van=0; van<lengte; ++van)
    {
      //Serial.println(van); //Debug
      //Zijn we al aan het einde?
      if (van + n_letters_per_regel >= lengte) 
      {
        Serial.println(tekst.substring(van,lengte));
        van = lengte;
        delay(500);
      } 
      if (van == lengte) break;
      //Lees de regel van 'van' tot 'tot' om de regel op een spatie af te breken
      for(int tot = van + n_letters_per_regel; tot > van; --tot)
      {
        //Serial.println(tot); //Debug
        if (tekst[tot] == ' ')
        {
          Serial.println(tekst.substring(van,tot));
          delay(500);
          van = tot;
          tot = van - 1;
          break;
        }
      }
    }
  }
}


void PrintHoofdstuk(const int hoofdstuk)
{
  PrintHoofdstuk1(hoofdstuk);  
}



