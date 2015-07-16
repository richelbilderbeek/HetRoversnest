#ifndef MONSTER_H
#define MONSTER_H

#include <string>

struct Monster
{
  Monster(
    const std::string& name,
    const int dexterity,
    const int stamina,
    const int attack_damage = 2
  );

  ///In a fight, produce a random attack strength
  int CalcAttackStrength() const noexcept;

  void ChangeStamina(const int delta_stamina) noexcept;

  ///If the monster hits the player, how much damage will be dealt. This dependends on the weapon
  int GetAttackDamage() const noexcept { return m_attack_damage; }



  int GetDexterity() const noexcept { return m_dexterity; }
  int GetInitialStamina() const noexcept { return m_initial_stamina; }
  std::string GetName() const noexcept { return m_name; }
  int GetStamina() const noexcept { return m_stamina; }
  bool IsDead() const noexcept { return m_stamina < 1; }

  private:
  const int m_attack_damage;
  const int m_dexterity;
  const int m_initial_stamina;
  const std::string m_name;
  int m_stamina;
};

std::ostream& operator<<(std::ostream& os, const Monster& monster);

#endif // MONSTER_H
