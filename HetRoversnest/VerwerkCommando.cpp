#include "VerwerkCommando.h"

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
  }
  Serial.println("");
}
