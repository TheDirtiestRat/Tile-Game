#include "../header/InventorySystem.hpp"

#include <algorithm>
#include <stdexcept>

namespace
{
constexpr int healingPotionAmount = 10;
}

const char* GetItemName(ItemType type)
{
    switch (type)
    {
    case ItemType::HealingPotion:
        return "Potion";
    case ItemType::Key:
        return "Key";
    }

    return "Unknown";
}

bool InventorySystem::AddItem(ItemType type, int quantity)
{
    if (quantity <= 0)
        throw std::invalid_argument("Item quantity must be positive.");

    const auto existing = std::find_if(
        items.begin(),
        items.end(),
        [type](const ItemStack& item)
        {
            return item.type == type;
        });

    if (existing != items.end())
    {
        existing->quantity += quantity;
        return true;
    }

    if (items.size() >= inventorySlotCount)
        return false;

    items.push_back({type, quantity});
    return true;
}

bool InventorySystem::HasItem(ItemType type, int quantity) const
{
    if (quantity <= 0)
        return false;

    const auto found = std::find_if(
        items.begin(),
        items.end(),
        [type, quantity](const ItemStack& item)
        {
            return item.type == type && item.quantity >= quantity;
        });

    return found != items.end();
}

bool InventorySystem::ConsumeItem(ItemType type, int quantity)
{
    if (quantity <= 0)
        return false;

    const auto found = std::find_if(
        items.begin(),
        items.end(),
        [type, quantity](const ItemStack& item)
        {
            return item.type == type && item.quantity >= quantity;
        });

    if (found == items.end())
        return false;

    found->quantity -= quantity;
    if (found->quantity == 0)
        items.erase(found);

    return true;
}

std::string InventorySystem::UseItem(std::size_t slot, PlayerStatsManager& stats)
{
    if (slot >= items.size())
        return "There is no item in that slot.";

    ItemStack& item = items[slot];
    if (item.type == ItemType::Key)
        return "Keys are used to unlock chests.";

    if (item.type == ItemType::HealingPotion)
    {
        const int healthBefore = stats.GetCurrentHealth();
        stats.Heal(healingPotionAmount);
        const int healthRestored = stats.GetCurrentHealth() - healthBefore;

        if (healthRestored == 0)
            return "Health is already full.";

        --item.quantity;
        if (item.quantity == 0)
            items.erase(items.begin() + static_cast<std::ptrdiff_t>(slot));

        return "Used a healing potion. Restored " +
               std::to_string(healthRestored) + " HP.";
    }

    return "This item cannot be used yet.";
}

const std::vector<ItemStack>& InventorySystem::GetItems() const
{
    return items;
}
