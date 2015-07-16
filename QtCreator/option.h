#ifndef OPTION_H
#define OPTION_H

#include <string>
#include <vector>

struct Character;

///An option the player may or may not choose
///Option [text]@ goto [chapter]@
///Option [text]@ if [conditionals] goto [chapter]@
///Option [text]@ then [consequences] goto [chapter]@
///Option [text]@ if [conditionals] then [consequences] goto [chapter]@
struct Option
{
  Option(
    const std::string& text,
    const int next_chapter
  );

  bool CanChoose(const Character& character);

  void DoChoose(Character& character) const;

  const std::string& GetText() const noexcept { return m_text; }

  private:
  int m_next_chapter;
  std::string m_text;
};

#endif // OPTION_H
