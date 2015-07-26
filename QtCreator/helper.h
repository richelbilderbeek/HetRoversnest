#ifndef HELPER_H
#define HELPER_H

#include <iosfwd>
#include <string>
#include <vector>

void CreateGraph();

///FileToString reads a file and converts it to a std::string
std::string FileToString(const std::string& filename);


///FileToVector reads a file and converts it to a std::vector<std::string>
///From http://www.richelbilderbeek.nl/CppFileToVector.htm
std::vector<std::string> FileToVector(const std::string& filename);

///Is x between a and b?
bool IsBetween(const double x, const double a, const double b);

bool IsInt(const std::string& s) noexcept;

///Determines if a filename is a regular file
///From http://www.richelbilderbeek.nl/CppIsRegularFile.htm
bool IsRegularFile(const std::string& filename);

void Parse(std::stringstream& s, const char expected_char);

char ReadChar(std::stringstream& s);
int ReadInt(std::stringstream& s);
std::string ReadString(std::stringstream& s);

///Read text until @
std::string ReadText(std::stringstream& s);

///Strip only the first character of the first line
std::vector<std::string> StripFirstChar(std::vector<std::string> v);

#ifndef NDEBUG
void TestHelperFunctions();
#endif // NDEBUG

///Replace 'two_words' by 'two words'
std::string ToPretty(std::string s);

void Wait(const double n_secs) noexcept;

#endif // HELPER_H
