#include "../header/PlayerStatsManager.hpp"

#include <algorithm>
#include <stdexcept>

PlayerStatsManager::PlayerStatsManager(int maxHealth, int attackPoints, int defensePoints)
    : maxHealth(maxHealth),
      currentHealth(maxHealth),
      baseAttackPoints(attackPoints),
      baseDefensePoints(defensePoints)
{
    if (maxHealth <= 0 || attackPoints < 0 || defensePoints < 0)
        throw std::invalid_argument("Player stats must have positive health and non-negative attack/defense.");
}

int PlayerStatsManager::GetMaxHealth() const
{
    return maxHealth;
}

int PlayerStatsManager::GetCurrentHealth() const
{
    return currentHealth;
}

int PlayerStatsManager::GetAttackPoints() const
{
    int points = baseAttackPoints;
    for (const StatusEffect& effect : statusEffects)
        points += effect.attackModifier;

    return std::max(0, points);
}

int PlayerStatsManager::GetDefensePoints() const
{
    int points = baseDefensePoints;
    for (const StatusEffect& effect : statusEffects)
        points += effect.defenseModifier;

    return std::max(0, points);
}

const std::vector<StatusEffect>& PlayerStatsManager::GetStatusEffects() const
{
    return statusEffects;
}

int PlayerStatsManager::TakeDamage(int damage)
{
    if (damage <= 0 || currentHealth <= 0)
        return 0;

    const int mitigatedDamage = std::max(1, damage - GetDefensePoints());
    const int previousHealth = currentHealth;
    currentHealth = std::max(0, currentHealth - mitigatedDamage);
    return previousHealth - currentHealth;
}

void PlayerStatsManager::Heal(int amount)
{
    if (amount < 0)
        throw std::invalid_argument("Healing amount cannot be negative.");

    currentHealth += std::min(amount, maxHealth - currentHealth);
}

void PlayerStatsManager::AddStatusEffect(const StatusEffect& effect)
{
    if (effect.name.empty() || effect.remainingSeconds <= 0.0f)
        throw std::invalid_argument("Status effects require a name and a positive duration.");

    const auto existing = std::find_if(
        statusEffects.begin(),
        statusEffects.end(),
        [&effect](const StatusEffect& current)
        {
            return current.name == effect.name;
        });

    if (existing != statusEffects.end())
        *existing = effect;
    else
        statusEffects.push_back(effect);
}

void PlayerStatsManager::Update(float dt)
{
    if (dt <= 0.0f)
        return;

    for (StatusEffect& effect : statusEffects)
        effect.remainingSeconds -= dt;

    statusEffects.erase(
        std::remove_if(
            statusEffects.begin(),
            statusEffects.end(),
            [](const StatusEffect& effect)
            {
                return effect.remainingSeconds <= 0.0f;
            }),
        statusEffects.end());
}
