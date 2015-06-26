#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <vector>

///FileToVector reads a file and converts it to a std::vector<std::string>
///From http://www.richelbilderbeek.nl/CppFileToVector.htm
std::vector<std::string> FileToVector(const std::string& filename);

///Determines if a filename is a regular file
///From http://www.richelbilderbeek.nl/CppIsRegularFile.htm
bool IsRegularFile(const std::string& filename);

#endif // HELPER_H
