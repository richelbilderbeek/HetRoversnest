#ifndef KRIJGTEKST_H
#define KRIJGTEKST_H

void KrijgTekst(const int hoofdstuk)
{
  const String bestandsnaam = String(hoofdstuk) + ".txt";

  if (!SD.exists(const_cast<char*>(bestandsnaam.c_str()))) {
    Serial.print(bestandsnaam); Serial.println(" niet gevonden!");
    return;
  }
  File file = SD.open(bestandsnaam.c_str(), FILE_READ);
  
  int pos = 0;
  while (file.available())
  {
    const char c = file.read();
    if (c == '\n') continue;
    Serial.print(c);
    ++pos;
    if (pos > 60 && c == ' ') 
    {
      Serial.println();
      pos = 0;
    }
  }
}

#endif // KRIJGTEKST_H

