#include <Arduino.h>
#include "KrijgTekst.h"

///Simpel
/*
void PrintHoofdstuk1(const int hoofdstuk)
{
  Serial.print("Hoofdstuk: ");
  Serial.println(hoofdstuk);
  
  for (int paragraaf = 0; ; ++paragraaf)
  {
    //Serial.print("Paragraaf: "); Serial.println(paragraaf); //DEBUG
    const char * const tekst = KrijgTekst(hoofdstuk,paragraaf);
    if (tekst == '\0') break; //Klaar
    Serial.println(tekst);
    delay(100000);
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
*/

void PrintHoofdstuk3(const int hoofdstuk)
{
  const int n_letters_per_regel = 40;
  
  for (int paragraaf = 0; ; ++paragraaf)
  {
    //Serial.print("Paragraaf: "); Serial.println(paragraaf); //DEBUG
    const char * const tekst = KrijgTekst(hoofdstuk,paragraaf);
    if (tekst == '\0') 
    {
      //Serial.print("Paragraaf heeft geen tekst"); //DEBUG
      return;
    }
    //Serial.print("Paragraaf heeft wel tekst"); //DEBUG
    for (int van=0; van >= 0; ++van)
    {
      //Serial.print("Paragraaf: "); Serial.print(paragraaf); Serial.print(", van: "); Serial.println(van); //Debug

      //Lees de regel van 'van' tot 'tot' om de regel op een spatie af te breken
      for(int tot = van + 1; tot >= 0; ++tot)
      {
        //Serial.print("Paragraaf: "); Serial.print(paragraaf); Serial.print(", van: "); Serial.print(van); Serial.print(", tot: "); Serial.println(tot); //Debug
        if (tekst[tot] == '\0')
        {
          //Print alles
          int n_achter_elkaar = 0;
          for (int i=van; i!=tot; ++i) 
          { 
            Serial.print(tekst[i]); 
            delay(10); 
            
            //Zorg ervoor dat we naar een volgende regel gaan als we teveel tekens hebben
            ++n_achter_elkaar;
            if (n_achter_elkaar > n_letters_per_regel && tekst[i] == ' ')
            {
              Serial.println();
              n_achter_elkaar = 0;
              
            }
          }
          Serial.println();
          
          //Serial.println("Volgende paragraaf");
          tot = -2; //-2, omdat ik uit de 'tot' for loop wil breken, maar er wordt ook nog een ++tot gedaan
          van = -2; 
          break;
        }
      }
    }
  }
}


void PrintHoofdstuk(const int hoofdstuk)
{
  PrintHoofdstuk3(hoofdstuk);  
}



