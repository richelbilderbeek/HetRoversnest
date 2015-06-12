#ifndef VERWERKCOMMANDO_H
#define VERWERKCOMMANDO_H

void VerwerkCommando(int& hoofdstuk, const int commando)
{
  if (commando < 1) return;
  switch (hoofdstuk)
  {
    case 1:
    {
      switch (commando)
      {
        case 1: hoofdstuk = 202; return;
        case 2: hoofdstuk = 33; return;
        case 3: hoofdstuk = 49; return;
      }
    }
    break;
    case 2:
    {
      switch (commando)
      {
        case 1: hoofdstuk = 377; return;
        case 2: hoofdstuk = 174; return;
      }
    }
    break;
  }
}

#endif // VERWERKCOMMANDO_H

