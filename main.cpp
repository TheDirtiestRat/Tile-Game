#include <algorithm>
#include <raylib.h>

#include "header/game_config.hpp"
#include "header/player.hpp"
#include "header/enemy.hpp"
#include "header/UserInterface.hpp"
#include "header/RoomManager.hpp"

// global variables
std::string outputLog = "Logs:";

// textures
Texture2D player_img;

// list of enemies
std::vector<Enemy> enemies;

namespace
{
void UpdateSelection(int& selectedSlot, int slotCount)
{
    if (slotCount <= 0)
    {
        selectedSlot = 0;
        return;
    }

    selectedSlot = std::max(0, std::min(selectedSlot, slotCount - 1));

    int nextSlot = selectedSlot;
    const int row = selectedSlot / inventoryColumns;
    const int col = selectedSlot % inventoryColumns;

    if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && col > 0)
        --nextSlot;
    else if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) &&
             col + 1 < inventoryColumns)
        ++nextSlot;
    else if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && row > 0)
        nextSlot -= inventoryColumns;
    else if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) &&
             row + 1 < inventoryRows)
        nextSlot += inventoryColumns;

    if (nextSlot < slotCount)
        selectedSlot = nextSlot;
}

ChestSpawn* FindNearbyChest(int row, int col)
{
    const int adjacentTiles[][2] = {
        {row, col},
        {row - 1, col},
        {row + 1, col},
        {row, col - 1},
        {row, col + 1}};

    for (const auto& tile : adjacentTiles)
    {
        ChestSpawn* chest = FindChestAt(tile[0], tile[1]);
        if (chest != nullptr)
            return chest;
    }

    return nullptr;
}

void TryOpenChest(
    ChestSpawn& chest,
    InventorySystem& inventory,
    ChestSpawn*& openChest,
    int& selectedChestSlot,
    std::string& outputLog)
{
    if (!chest.isOpen)
    {
        if (chest.requiredKeys > 0 &&
            !inventory.ConsumeItem(ItemType::Key, chest.requiredKeys))
        {
            outputLog = TextFormat(
                "This chest needs %d key(s).",
                chest.requiredKeys);
            return;
        }

        chest.isOpen = true;
        outputLog = "Chest unlocked.";
    }

    openChest = &chest;
    selectedChestSlot = 0;
}
}

int main()
{
    InitWindow(screenWidth, screenHeight, "Tile Game");
    SetTargetFPS(60);
    InitAudioDevice();

    // textures
    LoadRoomTextures();
    player_img = LoadTexture("img/player_05.png");
    Texture2D item_texture = LoadTexture("img/environment_11.png");
    Texture2D key_texture = LoadTexture("img/environment_10.png");
    Texture2D chest_texture = LoadTexture("img/crate_02.png");
    // sounds
    Sound sound_fx_1 = LoadSound("snd/can_open_1.mp3");

    // the player
    Player player(player_img, 30);
    InventorySystem inventory;
    bool inventory_open = false;
    ChestSpawn* open_chest = nullptr;
    int selected_inventory_slot = 0;
    int selected_chest_slot = 0;

    // load the starter room
    LoadRoom(0, player, enemies);

    while (!WindowShouldClose())
    {
        const float delta = GetFrameTime();

        if (open_chest != nullptr)
        {
            if (IsKeyPressed(KEY_ESCAPE))
            {
                open_chest = nullptr;
            }
            else if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_I))
            {
                open_chest = nullptr;
                inventory_open = true;
            }
            else
            {
                UpdateSelection(
                    selected_chest_slot,
                    static_cast<int>(open_chest->contents.size()));

                if (IsKeyPressed(KEY_ENTER) &&
                    selected_chest_slot >= 0 &&
                    selected_chest_slot < static_cast<int>(open_chest->contents.size()))
                {
                    ItemStack& item = open_chest->contents[selected_chest_slot];
                    if (inventory.AddItem(item.type))
                    {
                        outputLog = TextFormat("Took %s from the chest.", GetItemName(item.type));
                        --item.quantity;
                        if (item.quantity == 0)
                        {
                            open_chest->contents.erase(
                                open_chest->contents.begin() + selected_chest_slot);
                            selected_chest_slot = open_chest->contents.empty()
                                ? 0
                                : std::min(
                                      selected_chest_slot,
                                      static_cast<int>(open_chest->contents.size()) - 1);
                        }
                    }
                    else
                    {
                        outputLog = "Inventory is full.";
                    }
                }
            }
        }
        else if (inventory_open)
        {
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_E) || IsKeyPressed(KEY_I))
            {
                inventory_open = false;
            }
            else
            {
                UpdateSelection(selected_inventory_slot, inventorySlotCount);
                if (IsKeyPressed(KEY_ENTER))
                {
                    outputLog = inventory.UseItem(
                        static_cast<std::size_t>(selected_inventory_slot),
                        player.stats);
                }
            }
        }
        else
        {
            player.stats.Update(delta);

            if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_I))
            {
                inventory_open = true;
            }
            else if (IsKeyPressed(KEY_ENTER))
            {
                const int player_col = static_cast<int>(player.position.x / tileSize);
                const int player_row = static_cast<int>(player.position.y / tileSize);
                ChestSpawn* chest = FindNearbyChest(player_row, player_col);
                if (chest != nullptr)
                {
                    TryOpenChest(
                        *chest,
                        inventory,
                        open_chest,
                        selected_chest_slot,
                        outputLog);
                }
            }
            else
            {
                const Vector2 previous_position = player.position;
                player.Update(
                    delta,
                    terrain,
                    rows,
                    cols,
                    tileSize,
                    enemies);

                if (player.position.x != previous_position.x ||
                    player.position.y != previous_position.y)
                {
                    const int player_col = static_cast<int>(player.position.x / tileSize);
                    const int player_row = static_cast<int>(player.position.y / tileSize);
                    std::string itemMessage;

                    if (TryCollectItemAt(player_row, player_col, inventory, itemMessage))
                        outputLog = itemMessage;

                    ChestSpawn* chest = FindChestAt(player_row, player_col);
                    if (chest != nullptr)
                    {
                        TryOpenChest(
                            *chest,
                            inventory,
                            open_chest,
                            selected_chest_slot,
                            outputLog);
                    }
                }
            }

            if (open_chest == nullptr)
            {
                for (Enemy& enemy : enemies)
                {
                    enemy.Update(
                        delta,
                        player,
                        terrain,
                        rows,
                        cols,
                        tileSize);
                }

                const auto deadEnemiesEnd = std::remove_if(
                    enemies.begin(),
                    enemies.end(),
                    [](const Enemy& enemy)
                    {
                        return !enemy.isActive && enemy.damageFeedbackTimer <= 0.0f;
                    });

                if (deadEnemiesEnd != enemies.end())
                {
                    outputLog = "Enemy is slain...";
                    PlaySound(sound_fx_1);
                    enemies.erase(deadEnemiesEnd, enemies.end());
                }

                if (player.encounteredSomething)
                {
                    outputLog = player.whatEncountered;
                    player.encounteredSomething = false;
                }

                if (!player.isActive)
                    outputLog = "Player is dead...";

                RoomUpdate(player, enemies);
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        // the environment
        DrawRoomEnvironment();
        DrawRoomObjects(item_texture, key_texture, chest_texture);

        // the entities
        player.Draw();

        // draw all enemy
        for (Enemy &enemy : enemies)
        {
            enemy.Draw();
        }

        // the UI
        DrawUI(player.stats, outputLog);
        if (inventory_open)
            DrawInventoryUI(inventory, selected_inventory_slot, item_texture, key_texture);
        if (open_chest != nullptr)
            DrawChestUI(*open_chest, selected_chest_slot, item_texture, key_texture);

        EndDrawing();
    }
    // unload textures
    UnloadTexture(player_img);
    UnloadTexture(item_texture);
    UnloadTexture(key_texture);
    UnloadTexture(chest_texture);
    UnloadRoomTextures();
    // unload sounds
    UnloadSound(sound_fx_1);
    CloseWindow();
    return 0;
}