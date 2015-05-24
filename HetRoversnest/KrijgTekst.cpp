#include <Arduino.h>

String KrijgTekst1(const int paragraaf)
{
  Serial.print("KrijgTekst1 begin, paragraaf: ");
  Serial.println(paragraaf);
  String tekst;
  switch (paragraaf)
  {
    case 0: tekst = "De voettoch naar Zwartzand voert je zo'n vijfenzeventig kilometer "; break;
    case 1: tekst = "over vlakten en heuvels naar het Westen; "; break;
    case 2: tekst = "gelukkig zonder onaangename ontmoetingen. "; break;
    case 3: tekst = "Uiteindelijk bereik je de kust en zie je de hoge stadsmuur die zwartzand omgeeft "; break;
    case 4: tekst = "en de dichtopeengebouwde huizenklomp die de zee insteekt als een zwart onheilsteken. "; break;
    case 5: tekst = "Schepen liggen voor anker in de haven en rook stijgt langzaam op uit de schoorstenen. "; break;
    case 6: tekst = "Het ziet er vredig genoeg uit en pas als de wind van richting verandert, "; break;
    case 7: tekst = " ruik je het verval om het briesje "; break;
    case 8: tekst = "en het herinnert je aan de boze aard van dit beruchte oord. "; break;
    case 9: tekst = "Terwijl je de stoffige weg noordwaards langs de kust naar de stadspoorten volgt,"; break;
    case 10: tekst = "zie je de eerste angstaanjagende waarschuwingen: "; break;
    case 11: tekst =  "schedels op op houten staken, hongerende mannen in ijzeren kooien "; break;
    case 12: tekst =  "die aan de stadsmuur hangen en overal zwarte vlaggen"; break;
    case 13: tekst = "Als je de stadspoort nadert loopt er een koude rilling over je rug"; break;
    case 14: tekst = "en instinctief grijp je het gevest van je slagzwaard om jezelf gerust te stellen. "; break;
    case 15: tekst = "Bij de poort wordt je opgewacht door een lange wachter. Hij draagt een zwarte malienkolder en een ijzeren helm. "; break;
    case 16: tekst =  "Hij treedt naar voren, je de doorgang belemmerend met zijn piek en zegt: 'Wie waagt het onuitgenodigd naar Zwartzand te komen? "; break;
    case 17: tekst =  "Verklaar wat je hier te zoeken hebt of ga de weg terug die je gekomen bent.'. Wat doen je? "; break;
    case 18: tekst = "[1] Tegen hem zeggen dat je naar Nicodemus gebracht wil worden? "; break;
    case 19: tekst = "[2] Tegen hem zeggen dat je gestolen waar wil verkopen? "; break;
    case 20: tekst = "[3] Hem snel aanvallen met je zwaard? "; break;       
    
  }
  return tekst;
}

String KrijgTekst(const int hoofdstuk, const int paragraaf)
{
  Serial.print("KrijgTekst begin, hoofdstuk: ");
  Serial.print(hoofdstuk);
  Serial.print(", paragraaf: ");
  Serial.println(paragraaf);

  String tekst;
  switch (hoofdstuk)
  {
    case 1: return KrijgTekst1(paragraaf);
    case 33: if (paragraaf == 0) { tekst = "33"; }; break;
    case 49: if (paragraaf == 0) { tekst = "49"; } break;
    case 202: if (paragraaf == 0) { tekst = "202"; } break;
  } 
  return tekst;
}
