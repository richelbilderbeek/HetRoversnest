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
  assert(1==2);

  consoleDemoInit();
  videoSetMode(MODE_FB0);
  vramSetBankA(VRAM_A_LCD);

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
