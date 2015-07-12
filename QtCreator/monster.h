#ifndef MONSTER_H
#define MONSTER_H

#include <string>

struct Monster
{
  Monster(
    const std::string& name,
    const int dexterity,
    const int stamina
  );

  void ChangeStamina(const int delta_stamina) noexcept;
  bool IsDead() const noexcept { return m_stamina < 1; }

  private:
  const int m_dexterity;
  const int m_initial_stamina;
  const std::string m_name;
  int m_stamina;
};

#endif // MONSTER_H
