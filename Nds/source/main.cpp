#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

#include "ndsgamedialog.h"

#include <nds.h>
#include <dirent.h>

int main()
{
  #ifndef NDEBUG
  NO DEBUG MODE IN NDS
  #endif
  try
  {
    NdsGameDialog d;
    d.Start();
  }
  catch (std::logic_error& e)
  {
    std::cerr << e.what();
    std::cout << e.what();
  }
  catch (std::runtime_error& e)
  {
    std::cerr << e.what();
    std::cout << e.what();
  }
  while (1)
  {
    swiWaitForVBlank();

  }
}
