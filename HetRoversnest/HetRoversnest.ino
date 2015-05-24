#include "KrijgTekst.h"
#include "PrintHoofdstuk.h"
#include "VerwerkCommando.h"


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


void setup()
{
  Serial.begin(9600);  
  ///Het hoofdstuk in het boek
  int hoofdstuk = 1;

  if (KrijgTekst(1,0) == "") Serial.println("ERRROR");
  
  while (1)
  {
    PrintHoofdstuk(hoofdstuk);
    const int commando = LeesCommando();
    VerwerkCommando(hoofdstuk,commando);
  }
}

void loop() {}
