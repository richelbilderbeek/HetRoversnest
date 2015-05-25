#include <Arduino.h>

///Geeft de tekst van een hoofdstuk in een regel
///Elk hoofdstuk is in paragrafen verdeeld, omdat een String maar 256 (gok) tekens mag zijn
///Roep KrijgTekst aan met 'paragraaf' van 0 tot je niks meer krijgt
const char * KrijgTekst(const int hoofdstuk);

