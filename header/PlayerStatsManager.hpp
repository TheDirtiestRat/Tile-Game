#pragma once

#include <string>
#include <vector>

struct StatusEffect
{
    std::string name;
    float remainingSeconds;
    int attackModifier;
    int defenseModifier;
};

class PlayerStatsManager
{
public:
    explicit PlayerStatsManager(int maxHealth, int attackPoints = 3, int defensePoints = 2);

    int GetMaxHealth() const;
    int GetCurrentHealth() const;
    int GetAttackPoints() const;
    int GetDefensePoints() const;
    const std::vector<StatusEffect>& GetStatusEffects() const;

    int TakeDamage(int damage);
    void Heal(int amount);
    void AddStatusEffect(const StatusEffect& effect);
    void Update(float dt);

private:
    int maxHealth;
    int currentHealth;
    int baseAttackPoints;
    int baseDefensePoints;
    std::vector<StatusEffect> statusEffects;
};
