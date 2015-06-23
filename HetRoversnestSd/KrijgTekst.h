#ifndef KRIJGTEKST_H
#define KRIJGTEKST_H

const char * KrijgTekst(const int hoofdstuk)
{
  switch (hoofdstuk)
  {
    case 1: 
    { 
      const static char tekst[] = 
        "1"
       ;
      return tekst; 
    }
    case 2: 
    { 
      const static char tekst[] = 
        "2"  
      ;
      return tekst;
    }
    case 3:  
    {  
      const static char tekst[] = 
        "3"
      ;
      return tekst;
    }
  }
  return '\0';
}

#endif // KRIJGTEKST_H

