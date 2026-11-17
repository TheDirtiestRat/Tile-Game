#pragma once

#include <raylib.h>
#include <string>
#include "PlayerStatsManager.hpp"

class InventorySystem;
struct ChestSpawn;

void DrawUI(const PlayerStatsManager& stats, const std::string& outputLog);
void DrawInventoryUI(
    const InventorySystem& inventory,
    int selectedSlot,
    Texture2D itemTexture,
    Texture2D keyTexture);
void DrawChestUI(
    const ChestSpawn& chest,
    int selectedSlot,
    Texture2D itemTexture,
    Texture2D keyTexture);