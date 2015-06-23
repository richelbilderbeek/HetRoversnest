#ifndef LAATZIEN_H
#define LAATZIEN_H

#include <Arduino.h>
#include "KrijgTekst.h"

void LaatZien(const char * const tekst)
{
  const int vertraging_msec = 1;
  const int n_letters_per_regel = 40;
  
  if (tekst == '\0') 
  {
    return;
  }

  for (int van=0; van >= 0; ++van)
  {
    //Zoek het einde van de regel
    for(int tot = van + 1; tot >= 0; ++tot)
    {
      if (tekst[tot] == '\0') //Einde gevonden
      {
        //Print regel
        
        int n_achter_elkaar = 0;
        for (int i=van; i!=tot; ++i) 
        { 
          if (tekst[i] == '[')
          {
            Serial.println();
            n_achter_elkaar = 0;            
          }
          
          Serial.print(tekst[i]); 
          delay(vertraging_msec); 
          
          //Zorg ervoor dat we naar een volgende regel gaan als we teveel tekens hebben
          ++n_achter_elkaar;
          if (n_achter_elkaar > n_letters_per_regel && tekst[i] == ' ')
          {
            Serial.println();
            n_achter_elkaar = 0;
            
          }
        }
        Serial.println();
        
        tot = -2; //-2, omdat ik uit de 'tot' for loop wil breken, maar er wordt ook nog een ++tot gedaan
        van = -2; 
        break;
      }
    }
  }
}

#endif // LAATZIEN_H

