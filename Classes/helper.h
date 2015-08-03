#ifndef HELPER_H
#define HELPER_H

#include <iosfwd>
#include <string>
#include <vector>

struct Helper
{
  Helper();

  void Cout(const char c) const;
  void Cout(const std::string& s) const;
  void CoutNl(const std::string& s) const;

  void CreateGraph() const;

  #ifndef ARM9 //No files with NDS
  ///FileToString reads a file and converts it to a std::string
  std::string FileToString(const std::string& filename) const;

  ///FileToVector reads a file and converts it to a std::vector<std::string>
  ///From http://www.richelbilderbeek.nl/CppFileToVector.htm
  std::vector<std::string> FileToVector(const std::string& filename) const;

  ///What is the path of the files?
  std::string GetFilesFolder() const;
  #endif // ARM9

  ///Is x between a and b?
  bool IsBetween(const double x, const double a, const double b) const;

  bool IsInt(const std::string& s) const noexcept;

  ///Determines if a filename is a regular file
  ///From http://www.richelbilderbeek.nl/CppIsRegularFile.htm
  bool IsRegularFile(const std::string& filename) const;

  void Parse(std::stringstream& s, const char expected_char) const;

  char ReadChar(std::stringstream& s) const;
  int ReadInt(std::stringstream& s) const;
  std::string ReadString(std::stringstream& s) const;

  ///Read text until @
  std::string ReadText(std::stringstream& s) const;

  ///Strip only the first character of the first line
  std::vector<std::string> StripFirstChar(std::vector<std::string> v) const;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif // NDEBUG

  int ToInt(const std::string& s) const;

  ///Replace 'two_words' by 'two words'
  std::string ToPretty(std::string s) const;


  std::string ToStr(const int i) const;

  ///Trim leading and trailing whitespace
  //From http://www.richelbilderbeek.nl/CppTrim.htm
  std::string Trim(const std::string& s) const;

  void Wait(const double n_secs) const noexcept;
};

#endif // HELPER_H
