#ifndef MONSTER_H
#define MONSTER_H

#include <string>

struct Monster
{
  Monster(
    const std::string& name,
    const int dexterity,
    const int stamina,
    const int attack_strength = 2
  );

  void ChangeStamina(const int delta_stamina) noexcept;
  int GetAttackStrength() const noexcept { return m_attack_strength; }
  int GetDexterity() const noexcept { return m_dexterity; }
  std::string GetName() const noexcept { return m_name; }
  int GetStamina() const noexcept { return m_stamina; }
  bool IsDead() const noexcept { return m_stamina < 1; }

  private:
  const int m_attack_strength;
  const int m_dexterity;
  const int m_initial_stamina;
  const std::string m_name;
  int m_stamina;
};

std::ostream& operator<<(std::ostream& os, const Monster& monster);

#endif // MONSTER_H
