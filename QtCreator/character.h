#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>
#include "item.h"


struct Character
{
  using Items = std::vector<Item>;
  using MonsterNames = std::vector<std::string>;

  Character(
    const int skill, //NL: Behendigheid
    const int condition, //NL: Conditie
    const int luck, //NL: Geluk
    const Item initial_item
  );

  void AddHasFought(const std::string& monster_name);
  void AddItem(const Item item);

  ///In a fight, produce a random attack strength
  int CalcAttackStrength() const noexcept;

  void ChangeArrows(const int change);
  void ChangeSkill(const int change);
  void ChangeGold(const int change);
  void ChangeProvisions(const int change);
  void ChangeCondition(const int change);
  void ChangeLuck(const int change);
  int GetArrows() const noexcept { return m_arrows; }
  const std::vector<int>& GetChapters() const noexcept { return m_chapters; }
  int GetCurrentChapter() const noexcept { return m_chapters.back(); }
  int GetSkill() const noexcept;
  int GetSkillBase() const noexcept;
  int GetGold() const noexcept { return m_gold; }
  const Items& GetItems() const noexcept { return m_items; }
  int GetInitialSkill() const noexcept { return m_initial_skill; }
  int GetInitialLuck() const noexcept { return m_initial_luck; }
  int GetInitialCondition() const noexcept { return m_initial_condition; }
  int GetLuck() const noexcept;
  int GetLuckBase() const noexcept;
  int GetProvisions() const noexcept { return m_provisions; }
  int GetCondition() const noexcept { return m_condition; }
  bool HasFought(const std::string& monster_name) const;
  bool HasItem(const Item item) const;
  bool IsDead() const noexcept { return m_condition <= 0; }
  void RemoveItem(const Item item);
  void SetChapter(const int chapter);
  void SetIsDead() noexcept { m_condition = 0; }
  std::string ShowInventory();
  bool TestSkill() noexcept;
  bool TestLuck() noexcept;

  private:
  std::vector<int> m_chapters;

  ///How much arrows does the character have sticking out his/her body?
  int m_arrows;
  int m_skill; //NL: Behendigheid
  MonsterNames m_fought;
  int m_gold; //NL: Goud
  const int m_initial_skill; //NL: Behendigheid
  const int m_initial_luck; //NL: Geluk
  const int m_initial_condition; //NL: Conditie
  Items m_items; //NL: Voorwerpen
  int m_luck; //NL: Geluk
  int m_provisions; //NL: Proviant
  int m_condition; //NL: Conditie
};

#endif // CHARACTER_H
