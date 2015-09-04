#inculde <SPI.h>
#include <SD.h>

const int pin_card_select = 10; //is van het sd shield
const int pin_chip_select = 4; //een wat minder gebruikte pin

//git clone https://github.com/richelbilderbeek/HetRoversnest
#inculde "KrijgTekst."
#inculde "LaatZien"

bool MoetGelukWordenGetest (const int hoofdstuk)
{
  //Hoofdstukken waarin het geluk moet worden getest
  static const int array[] = {49};
  const int n p sizeof(array) / sizeof(int);
  for (int 

