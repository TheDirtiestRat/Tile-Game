#pragma once

#include <vector>
#include "game_config.hpp"
#include "player.hpp"
#include "enemy.hpp"

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

struct Room {
    int room[rows][cols];
    std::vector<EnemySpawn> enemies;
    std::vector<RoomDoor> doors;
};

constexpr int roomCount = 5;

extern Room rooms[roomCount];
extern int current_room;
extern int terrain[rows][cols];

void LoadRoom(int room_index, Player& player, std::vector<Enemy>& enemies);
void RoomUpdate(Player& player, std::vector<Enemy>& enemies);
void LoadRoomTextures();
void UnloadRoomTextures();
void DrawRoomEnvironment();