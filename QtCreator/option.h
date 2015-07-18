#ifndef OPTION_H
#define OPTION_H

#include <string>
#include <vector>

#include "condition.h"
#include "consequence.h"
#include "item.h"
struct Character;

///An option the player may or may not choose
///Option [text]@ goto [chapter]@
///Option [text]@ if [conditionals] goto [chapter]@
///Option [text]@ then [consequences] goto [chapter]@
///Option [text]@ if [conditionals] change [consequences] goto [chapter]@
struct Option
{
  Option(
    const std::string& text,
    const Consequence& consequence
  );

  void AddCondition(const Condition& condition);

  bool CanChoose(const Character& character) const;

  void DoChoose(Character& character) const;

  const std::vector<Condition>& GetConditions() const noexcept { return m_conditions; }

  int GetNextChapter() const noexcept { return m_consequence.GetNextChapter(); }

  const std::string& GetText() const noexcept { return m_text; }


  private:
  std::vector<Condition> m_conditions;
  Consequence m_consequence;
  //int m_next_chapter;
  std::string m_text;
};

std::ostream& operator<<(std::ostream& os, const Option& option);

#endif // OPTION_H
