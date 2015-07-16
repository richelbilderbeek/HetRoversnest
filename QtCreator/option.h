#ifndef OPTION_H
#define OPTION_H

#include <string>
#include <vector>

struct Character;

///An option the player may or may not choose
///Option If [conditionals] Then [consequences] [text] @
struct Option
{
  Option(
    const std::vector<std::string>& conditionals,
    const std::vector<std::string>& consequences,
    const std::string& text
  );

  bool CanChoose(const Character& character);
  bool DoChoose(Character& character);

  const std::vector<std::string>& GetConditionals() const noexcept { return m_conditionals; }
  const std::vector<std::string>& GetConsequences() const noexcept { return m_consequences; }
  const std::string& GetText() const noexcept { return m_text; }

  private:
  const std::vector<std::string> m_conditionals;
  const std::vector<std::string> m_consequences;
  const std::string m_text;
};

#endif // OPTION_H
