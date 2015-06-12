#ifndef KRIJGTEKST_H
#define KRIJGTEKST_H

const char * KrijgTekst(const int hoofdstuk)
{
  switch (hoofdstuk)
  {
    case 1: 
    { 
      const static char tekst[] = 
        "De voettoch naar Zwartzand voert je zo'n vijfenzeventig kilometer "
        "over vlakten en heuvels naar het Westen; "
        "gelukkig zonder onaangename ontmoetingen. "
        "Uiteindelijk bereik je de kust en zie je de hoge stadsmuur die zwartzand omgeeft "
        "en de dichtopeengebouwde huizenklomp die de zee insteekt als een zwart onheilsteken. "
        "Schepen liggen voor anker in de haven en rook stijgt langzaam op uit de schoorstenen. "
        "Het ziet er vredig genoeg uit en pas als de wind van richting verandert, "
        "ruik je het verval om het briesje "
        "en het herinnert je aan de boze aard van dit beruchte oord. "
        "Terwijl je de stoffige weg noordwaards langs de kust naar de stadspoorten volgt,"
        "zie je de eerste angstaanjagende waarschuwingen: "
        "schedels op op houten staken, hongerende mannen in ijzeren kooien "
        "die aan de stadsmuur hangen en overal zwarte vlaggen. "
        "Als je de stadspoort nadert loopt er een koude rilling over je rug"
        "en instinctief grijp je het gevest van je slagzwaard om jezelf gerust te stellen. "
        "Bij de poort wordt je opgewacht door een lange wachter. Hij draagt een zwarte malienkolder en een ijzeren helm. "
        "Hij treedt naar voren, je de doorgang belemmerend met zijn piek en zegt: 'Wie waagt het onuitgenodigd naar Zwartzand te komen? "
        "Verklaar wat je hier te zoeken hebt of ga de weg terug die je gekomen bent.'. "
        "Wat doen je? "
        "[1] Tegen hem zeggen dat je naar Nicodemus gebracht wil worden? "
        "[2] Tegen hem zeggen dat je gestolen waar wil verkopen? "
        "[3] Hem snel aanvallen met je zwaard? "
       ;
      return tekst; 
    }

    case 6: 
    { 
      const static char tekst[] = 
        "Haar toon wordt onvriendelijk en ze snauwi je toe je weg te scheren omdat er in háár "
        "huis geen vodden 1e vinden zijn,  en trouwens helemaal geen rommel.  Als je wilt wat ze zegt,  verlaat dan het huis en loop verder noord-  r waarts door de Stalstraat:  ga naar 333 Als je achter de dijnen wilt kijken wie je zo lomp bejegent,  dan naar 88 "
      ;
      return tekst;
    }

    case 33: 
    { 
      const static char tekst[] = 
        "33"
      ;
      return tekst;
    }
    case 49: 
    { 
      const static char tekst[] = 
        "Als je je zwaard trekt, duikt de wachter naar rechts "
        "in een poging een kleine bel aan de muur van het wachthuis te luiden. "
      ;
      return tekst;
    }
    case 202: 
    { 
      const static char tekst[] = 
        "202"
      ;
      return tekst;
    }
  }
  return '\0';
}

#endif // KRIJGTEKST_H

