#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "PlayerStatsManager.hpp"

constexpr int inventoryColumns = 5;
constexpr int inventoryRows = 4;
constexpr int inventorySlotCount = inventoryColumns * inventoryRows;

enum class ItemType
{
    HealingPotion,
    Key
};

struct ItemStack
{
    ItemType type;
    int quantity;
};

const char* GetItemName(ItemType type);

class InventorySystem
{
public:
    bool AddItem(ItemType type, int quantity = 1);
    bool HasItem(ItemType type, int quantity = 1) const;
    bool ConsumeItem(ItemType type, int quantity = 1);
    std::string UseItem(std::size_t slot, PlayerStatsManager& stats);

    const std::vector<ItemStack>& GetItems() const;

private:
    std::vector<ItemStack> items;
};
