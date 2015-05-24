#include "KrijgTekst.h"

///Het hoofdstuk in het boek
int hoofdstuk = 1;

int LeesCommando()
{
  //Wacht op input
  while (!Serial.available())
  {
    delay(100); 
  }
  //Verwerk input
  String s;
  while (Serial.available())
  {
    s = s + Serial.read();
  } 
  return s.toInt();
}

///Laat het hoofdstuk op een mooie manier zien
void PrintHoofdstuk(const int hoofdstuk)
{
  const int n_letters_per_regel = 80;
  
  for (int paragraaf = 0; ; ++paragraaf)
  {
    const String tekst = KrijgTekst(hoofdstuk,paragraaf);
    if (tekst == "") break; //Klaar

 
    const int lengte = tekst.length();
    for (int van=0; van<lengte; ++van)
    {
      //Serial.println(van); //Debug
      //Zijn we al aan het einde?
      if (van + n_letters_per_regel >= lengte) 
      {
        Serial.println(tekst.substring(van,lengte));
        van = lengte;
      } 
      if (van == lengte) break;
      //Lees de regel van 'van' tot 'tot' om de regel op een [ af te breken
      /*
      for(int i=0; i!=n_letters_per_regel; ++i)
      {
        if (tekst[van + i] == '[')
        {
          Serial.println(tekst.substring(van,van+i-1));
          van = van + i - 1;
          goto klaar;
        }
      }
      */
      //Lees de regel van 'van' tot 'tot' om de regel op een spatie af te breken
      for(int tot = van + n_letters_per_regel; tot > van; --tot)
      {
        //Serial.println(tot); //Debug
        if (tekst[tot] == ' ')
        {
          Serial.println(tekst.substring(van,tot));
          van = tot;
          tot = van - 1;
          break;
        }
      }
    }
  }
}

void VerwerkCommando(int& hoofdstuk, const int commando)
{

}

void setup()
{
  Serial.begin(9600);  
}

void loop()
{
  PrintHoofdstuk(hoofdstuk);
  const int commando = LeesCommando();
  VerwerkCommando(hoofdstuk,commando);
}
