#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

#include <nds.h>

#include "ndsgamedialog.h"
#include "helper.h"

int main(int argc, char* argv[0])
{
  #ifndef NDEBUG
  NO DEBUG MODE IN NDS
  #endif

  consoleDemoInit();
  videoSetMode(MODE_FB0);
  vramSetBankA(VRAM_A_LCD);


  /*
  videoSetMode(MODE_0_2D);
  videoSetModeSub(MODE_0_2D);

  vramSetBankA(VRAM_A_MAIN_BG);
  vramSetBankC(VRAM_C_SUB_BG);
  */

  try
  {
    NdsGameDialog d(argc,argv);
    d.Start();
  }
  catch (std::logic_error& e)
  {
    Helper().Cout(e.what());
  }
  catch (std::runtime_error& e)
  {
    Helper().Cout(e.what());
  }
  while (1)
  {
    swiWaitForVBlank();

  }
}
