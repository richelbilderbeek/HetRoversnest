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

///Strip only the first character of the first line
std::vector<std::string> StripFirstChar(std::vector<std::string> v);

///Shows the text slowly
void ShowText(const std::string& text, const bool auto_play);

void Test();

///Replace 'two_words' by 'two words'
std::string ToPretty(std::string s);

void Wait(const double n_secs) noexcept;

#endif // HELPER_H
