#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

#include <filesystem.h>
#include <nds.h>
#include <dirent.h>

bool IsRegularFile(const std::string& filename)
{
  std::fstream f;
  f.open(filename.c_str(),std::ios::in);
  return f.is_open();
}

std::vector<std::string> FileToVector(const std::string& filename)
{
  assert(IsRegularFile(filename));
  std::vector<std::string> v;
  std::ifstream in(filename.c_str());
  for (int i=0; !in.eof(); ++i)
  {
    std::string s;
    std::getline(in,s);
    v.push_back(s);
  }
  return v;
}

std::string FileToString(const std::string& filename)
{
  if (!IsRegularFile(filename))
  {
    std::stringstream msg;
    msg << __func__ << ": ERROR: File " << filename << " does not exist";
    throw std::runtime_error(msg.str());
  }
  const std::vector<std::string> lines = FileToVector(filename);
  std::stringstream s;
  std::copy(std::begin(lines),std::end(lines),std::ostream_iterator<std::string>(s,"\n"));
  return s.str();
}


int main()
{
  nitroFSInit(nullptr);

	PrintConsole topScreen;
	PrintConsole bottomScreen;
	
	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankC(VRAM_C_SUB_BG);

  consoleInit(&topScreen,    3,BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
	consoleInit(&bottomScreen, 3,BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);

	consoleSelect(&topScreen);

  try
  {
    std::cout << FileToString("1.txt") << std::endl;
  }
  catch (std::runtime_error& e)
  {
    std::cout << e.what() << std::endl;
  }

	consoleSelect(&bottomScreen);

  try
  {
    std::cout << FileToString("2.txt") << std::endl;
  }
  catch (std::runtime_error& e)
  {
    std::cout << e.what() << std::endl;
  }

  while(1)
  {
    swiWaitForVBlank();
	}
}
