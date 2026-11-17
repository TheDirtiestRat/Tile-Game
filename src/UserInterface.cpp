#include <raylib.h>
#include <string>
#include "../header/game_config.hpp"
#include "../header/text_utils.hpp"
#include "../header/InventorySystem.hpp"
#include "../header/RoomManager.hpp"
#include "../header/UserInterface.hpp"

namespace
{
Texture2D GetTextureForItem(
    ItemType type,
    Texture2D itemTexture,
    Texture2D keyTexture)
{
    return type == ItemType::Key ? keyTexture : itemTexture;
}

void DrawItemGrid(
    const std::vector<ItemStack>& items,
    int selectedSlot,
    const char* title,
    const char* instructions,
    Texture2D itemTexture,
    Texture2D keyTexture)
{
    constexpr int panelWidth = 5 * tileSize;
    constexpr int panelHeight = 4 * tileSize;
    constexpr int columns = 5;
    constexpr int slotWidth = 56;
    constexpr int slotHeight = 44;
    constexpr int gap = 4;

    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.65f));

    const float panelX = (screenWidth - panelWidth) / 2.0f;
    const float panelY = (screenHeight - panelHeight) / 2.0f;
    Rectangle panel = {panelX, panelY, panelWidth, panelHeight};
    DrawRectangleRec(panel, DARKGRAY);
    DrawRectangleLinesEx(panel, 2, WHITE);
    DrawText(title, static_cast<int>(panelX + 8), static_cast<int>(panelY + 6), 18, WHITE);
    DrawText(instructions, static_cast<int>(panelX + 8), static_cast<int>(panelY + 26), 11, LIGHTGRAY);

    const int gridX = static_cast<int>(panelX + 8);
    const int gridY = static_cast<int>(panelY + 42);
    if (items.empty())
    {
        DrawText("Empty", static_cast<int>(panelX + 8), static_cast<int>(panelY + 65), 14, LIGHTGRAY);
    }

    for (int slot = 0; slot < inventorySlotCount; ++slot)
    {
        const int row = slot / columns;
        const int col = slot % columns;
        Rectangle slotRect = {
            static_cast<float>(gridX + col * (slotWidth + gap)),
            static_cast<float>(gridY + row * (slotHeight + gap)),
            slotWidth,
            slotHeight};

        DrawRectangleRec(slotRect, slot == selectedSlot ? DARKBLUE : GRAY);
        DrawRectangleLinesEx(slotRect, slot == selectedSlot ? 2.0f : 1.0f,
                             slot == selectedSlot ? YELLOW : LIGHTGRAY);

        if (slot >= static_cast<int>(items.size()))
            continue;

        const ItemStack& item = items[slot];
        const Texture2D texture = GetTextureForItem(item.type, itemTexture, keyTexture);
        if (texture.id != 0)
        {
            const Rectangle source = {0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)};
            const Rectangle destination = {slotRect.x + 3, slotRect.y + 7, 28, 28};
            DrawTexturePro(texture, source, destination, {0, 0}, 0, WHITE);
        }

        DrawText(GetItemName(item.type), static_cast<int>(slotRect.x + 32),
                 static_cast<int>(slotRect.y + 7), 10, WHITE);
        DrawText(TextFormat("x%d", item.quantity), static_cast<int>(slotRect.x + 32),
                 static_cast<int>(slotRect.y + 24), 11, WHITE);
    }
}
}

void DrawUI(const PlayerStatsManager& stats, const std::string& outputLog)
{
    constexpr int fontSize = 14;
    constexpr int statsX = 8;
    const int panelY = rows * tileSize + 4;

    DrawText(
        TextFormat("HP: %d / %d", stats.GetCurrentHealth(), stats.GetMaxHealth()),
        statsX,
        panelY + 2,
        fontSize,
        WHITE);
    DrawText(
        TextFormat("ATK: %d", stats.GetAttackPoints()),
        statsX,
        panelY + 19,
        fontSize,
        WHITE);
    DrawText(
        TextFormat("DEF: %d", stats.GetDefensePoints()),
        statsX,
        panelY + 36,
        fontSize,
        WHITE);

    Rectangle logBox = {166, static_cast<float>(panelY), 260, 56};

    DrawRectangleRec(logBox, DARKGRAY);
    DrawRectangleLinesEx(logBox, 1, WHITE);
    DrawTextWrapped(
        outputLog.c_str(),
        {logBox.x + 5, logBox.y + 5, logBox.width - 10, logBox.height - 10},
        fontSize,
        1,
        WHITE);

    constexpr int effectsX = 436;
    const std::vector<StatusEffect>& effects = stats.GetStatusEffects();
    DrawText(
        TextFormat("Effects (%d):", static_cast<int>(effects.size())),
        effectsX,
        panelY + 1,
        fontSize,
        WHITE);

    if (effects.empty())
    {
        DrawText("None", effectsX, panelY + 20, fontSize, LIGHTGRAY);
        return;
    }

    constexpr int visibleEffects = 3;
    for (int i = 0; i < visibleEffects && i < static_cast<int>(effects.size()); ++i)
    {
        const StatusEffect& effect = effects[i];
        DrawText(
            TextFormat("%s %.1fs", effect.name.c_str(), effect.remainingSeconds),
            effectsX,
            panelY + 17 + i * 14,
            12,
            LIGHTGRAY);
    }

}

void DrawInventoryUI(
    const InventorySystem& inventory,
    int selectedSlot,
    Texture2D itemTexture,
    Texture2D keyTexture)
{
    DrawItemGrid(
        inventory.GetItems(),
        selectedSlot,
        "Inventory",
        "Enter: use item   E/I or Esc: close",
        itemTexture,
        keyTexture);
}

void DrawChestUI(
    const ChestSpawn& chest,
    int selectedSlot,
    Texture2D itemTexture,
    Texture2D keyTexture)
{
    DrawItemGrid(
        chest.contents,
        selectedSlot,
        "Chest",
        "Enter: take item   E/I or Esc: close",
        itemTexture,
        keyTexture);
}