#include <raylib.h>
#include <vector>
#include "../header/RoomManager.hpp"
#include "../header/game_config.hpp"
#include "../header/player.hpp"
#include "../header/enemy.hpp"

//variables
int WALL = 1;
int GROUND = 0;
int SPAWN = 9;

//textures
Texture2D ground_texture;
Texture2D wall_texture;
Texture2D door_texture;
Texture2D enemy_texture;

Room rooms[roomCount] = {
    {
        {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 6, 0, 1, 0, 8, 1},
            {1, 0, 0, 1, 0, 0, 0, 0, 0, 8},
            {1, 1, 1, 1, 0, 6, 1, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 1, 1, 1, 1},
            {1, 9, 0, 0, 0, 0, 0, 0, 6, 1},
            {1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        },
        {
            {1, 4, 10, false},
            {5, 8, 5, true},
            {3, 5, 7, true}
        },
        {
            {2, 9, 2},
            {1, 8, 3}
        },
        {
            {2, 1, ItemType::HealingPotion, 2, false},
            {5, 2, ItemType::Key, 1, false}
        },
        {
            {4, 2, 0, false, {{ItemType::HealingPotion, 2}}},
            {1, 7, 1, false, {{ItemType::HealingPotion, 1}, {ItemType::Key, 1}}}
        }
    },
    {
        {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 1, 0, 0, 1, 1, 0, 1},
            {1, 0, 0, 0, 6, 0, 0, 0, 8, 1},
            {1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1, 6, 1},
            {1, 0, 0, 0, 0, 0, 6, 1, 0, 1},
            {1, 9, 0, 0, 0, 0, 0, 1, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 8, 1},
        },
        {
            {4, 8, 10, false},
            {5, 6, 5, true},
            {2, 4, 8, true}
        },
        {
            {7, 8, 4},
            {2, 8, 0}
        },
        {
            {3, 2, ItemType::HealingPotion, 1, false},
            {6, 2, ItemType::Key, 1, false}
        },
        {
            {4, 1, 1, false, {{ItemType::HealingPotion, 2}}},
            {5, 4, 0, false, {{ItemType::Key, 1}}}
        }
    },
    {
        {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 9, 0, 0, 0, 1, 0, 0, 8, 1},
            {1, 0, 1, 1, 0, 1, 0, 1, 0, 1},
            {1, 0, 0, 6, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 0, 1, 1, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 8, 1},
            {1, 0, 6, 0, 0, 1, 0, 6, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        },
        {
            {6, 2, 10, false},
            {6, 7, 5, true},
            {3, 3, 7, true}
        },
        {
            {5, 8, 3},
            {1, 8, 4}
        },
        {
            {2, 4, ItemType::HealingPotion, 1, false},
            {6, 1, ItemType::Key, 1, false}
        },
        {
            {4, 5, 1, false, {{ItemType::HealingPotion, 1}, {ItemType::Key, 1}}},
            {5, 1, 0, false, {{ItemType::HealingPotion, 2}}}
        }
    },
    {
        {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 9, 0, 0, 1, 0, 0, 0, 8, 1},
            {1, 0, 1, 0, 1, 0, 1, 1, 0, 1},
            {1, 0, 1, 0, 0, 0, 6, 0, 0, 1},
            {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
            {1, 0, 0, 0, 0, 0, 8, 1, 0, 1},
            {1, 0, 6, 0, 6, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        },
        {
            {6, 2, 10, false},
            {6, 4, 5, true},
            {3, 6, 8, true}
        },
        {
            {1, 8, 0},
            {5, 6, 4}
        },
        {
            {2, 3, ItemType::HealingPotion, 1, false},
            {6, 1, ItemType::Key, 1, false}
        },
        {
            {5, 4, 1, false, {{ItemType::HealingPotion, 2}}},
            {4, 8, 0, false, {{ItemType::Key, 1}}}
        }
    },
    {
        {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 9, 0, 0, 0, 0, 1, 0, 8, 1},
            {1, 0, 1, 1, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 6, 1, 1, 1, 0, 1},
            {1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 8, 1},
            {1, 0, 6, 0, 0, 0, 6, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        },
        {
            {6, 2, 10, false},
            {6, 6, 5, true},
            {3, 4, 7, true}
        },
        {
            {1, 8, 1},
            {5, 8, 2}
        },
        {
            {2, 4, ItemType::HealingPotion, 1, false},
            {6, 1, ItemType::Key, 1, false}
        },
        {
            {4, 5, 1, false, {{ItemType::HealingPotion, 1}, {ItemType::Key, 1}}},
            {5, 2, 0, false, {{ItemType::HealingPotion, 2}}}
        }
    }
};

// environments
int current_room = 0;
int terrain[rows][cols] = {};

void SetUpRoom(Player& player, const Room& room, std::vector<Enemy>& enemies)
{
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (terrain[row][col] == SPAWN) {
                player.position = {
                    static_cast<float>(col * tileSize),
                    static_cast<float>(row * tileSize)
                };
                player.nextDirx = col;
                player.nextDiry = row;
            }
        }
    }

    enemies.clear();
    enemies.reserve(room.enemies.size());
    for (const EnemySpawn& spawn : room.enemies) {
        enemies.emplace_back(enemy_texture, spawn.randomMove, spawn.healthPoints);
        enemies.back().position = {
            static_cast<float>(spawn.col * tileSize),
            static_cast<float>(spawn.row * tileSize)
        };
    }
}

void LoadRoom(int room_index, Player& player, std::vector<Enemy>& enemies)
{
    if (room_index < 0 || room_index >= roomCount) {
        return;
    }

    current_room = room_index;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            terrain[row][col] = rooms[current_room].room[row][col];
        }
    }

    SetUpRoom(player, rooms[current_room], enemies);
}

void RoomUpdate(Player& player, std::vector<Enemy>& enemies)
{
    const int player_col = static_cast<int>(player.position.x / tileSize);
    const int player_row = static_cast<int>(player.position.y / tileSize);

    if (player_row < 0 || player_row >= rows ||
        player_col < 0 || player_col >= cols)
        return;

    for (const RoomDoor& door : rooms[current_room].doors) {
        if (player_row == door.row && player_col == door.col) {
            player.Encounter("That is a door.");
            LoadRoom(door.destinationRoom, player, enemies);
            return;
        }
    }
}

bool TryCollectItemAt(int row, int col, InventorySystem& inventory, std::string& message)
{
    for (ItemSpawn& item : rooms[current_room].items) {
        if (item.collected || item.row != row || item.col != col)
            continue;

        if (!inventory.AddItem(item.type, item.quantity)) {
            message = "Inventory is full. The item remains in the room.";
            return true;
        }

        item.collected = true;
        message = TextFormat("Collected %s x%d.", GetItemName(item.type), item.quantity);
        return true;
    }

    return false;
}

ChestSpawn* FindChestAt(int row, int col)
{
    for (ChestSpawn& chest : rooms[current_room].chests) {
        if (chest.row == row && chest.col == col)
            return &chest;
    }

    return nullptr;
}

void DrawRoomObjects(Texture2D itemTexture, Texture2D keyTexture, Texture2D chestTexture)
{
    for (const ItemSpawn& item : rooms[current_room].items) {
        if (item.collected)
            continue;

        const Texture2D texture = item.type == ItemType::Key ? keyTexture : itemTexture;
        DrawTexture(texture, item.col * tileSize, item.row * tileSize, WHITE);
    }

    for (const ChestSpawn& chest : rooms[current_room].chests) {
        DrawTexture(
            chestTexture,
            chest.col * tileSize,
            chest.row * tileSize,
            chest.isOpen ? GRAY : WHITE);
    }
}

void LoadRoomTextures() {
    ground_texture = LoadTexture("img/ground_06.png");
    wall_texture = LoadTexture("img/block_02.png");
    door_texture = LoadTexture("img/crate_07.png");
    enemy_texture = LoadTexture("img/player_08.png");
}

void UnloadRoomTextures() {
    UnloadTexture(ground_texture);
    UnloadTexture(wall_texture);
    UnloadTexture(door_texture);
    UnloadTexture(enemy_texture);
}

void DrawRoomEnvironment()
{
    // draw the wall and ground
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            if ((terrain[row][col] == 1))
            {
                DrawTexture(wall_texture, col * tileSize, row * tileSize, WHITE);
            }
            else if ((terrain[row][col] == 8))
            {
                DrawTexture(door_texture, col * tileSize, row * tileSize, WHITE);
            }
            else
            {
                DrawTexture(ground_texture, col * tileSize, row * tileSize, WHITE);
            }
        }
    }
}