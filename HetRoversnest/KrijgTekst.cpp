#include <Arduino.h>

String KrijgTekst(const int hoofdstuk, const int paragraaf)
{
  String tekst;
  switch (hoofdstuk)
  {
    case 1: 
    
      switch (paragraaf)
      {
        case 0:
          tekst =  
            "De voettoch naar Zwartzand voert je zo'n vijfenzeventig kilometer over vlakten en heuvels naar het Westen; "
            "gelukkig zonder onaangename ontmoetingen. "
            "Uiteindelijk bereik je de kust en zie je de hoge stadsmuur die zwartzand omgeeft "
            "en de dichtopeengebouwde huizenklomp die de zee insteekt als een zwart onheilsteken. "
          ;
        break;
        case 1:
          tekst =  
            "Bij de poort wordt je opgewacht door een lange wachter. "
            "Hij draagt een zwarte malienkolder en een ijzeren helm. "
            "Hij treedt naar voren, je de doorgang belemmerend met zijn piek "
            "en zegt: 'Wie waagt het onuitgenodigd naar Zwartzand te komen? "
            "Verklaar wat je hier te zoeken hebt of ga de weg terug die je gekomen bent.'. "
            "Wat doen je? "
          ;
          break;
        case 2: tekst = "[1] Tegen hem zeggen dat je naar Nicodemus gebracht wil worden? ";
          break;
        case 3: tekst = "[2] Tegen hem zeggen dat je gestolen waar wil verkopen? ";
          break;
        case 4: tekst = "[3] Hem snel aanvallen met je zwaard? ";
          break;       
      }
    
  } 
  return tekst;
}
