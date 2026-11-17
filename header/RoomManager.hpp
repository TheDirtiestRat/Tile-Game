#pragma once

#include <vector>
#include "game_config.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "InventorySystem.hpp"

struct EnemySpawn {
    int row;
    int col;
    int healthPoints;
    bool randomMove;
};

struct RoomDoor {
    int row;
    int col;
    int destinationRoom;
};

struct ItemSpawn {
    int row;
    int col;
    ItemType type;
    int quantity;
    bool collected;
};

struct ChestSpawn {
    int row;
    int col;
    int requiredKeys;
    bool isOpen;
    std::vector<ItemStack> contents;
};

struct Room {
    int room[rows][cols];
    std::vector<EnemySpawn> enemies;
    std::vector<RoomDoor> doors;
    std::vector<ItemSpawn> items;
    std::vector<ChestSpawn> chests;
};

constexpr int roomCount = 5;

extern Room rooms[roomCount];
extern int current_room;
extern int terrain[rows][cols];

void LoadRoom(int room_index, Player& player, std::vector<Enemy>& enemies);
void RoomUpdate(Player& player, std::vector<Enemy>& enemies);
bool TryCollectItemAt(int row, int col, InventorySystem& inventory, std::string& message);
ChestSpawn* FindChestAt(int row, int col);
void DrawRoomObjects(Texture2D itemTexture, Texture2D keyTexture, Texture2D chestTexture);
void LoadRoomTextures();
void UnloadRoomTextures();
void DrawRoomEnvironment();