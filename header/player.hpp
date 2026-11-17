#pragma once

#include <raylib.h>
#include <vector>
#include <string>

#include "game_config.hpp"
#include "enemy.hpp"

struct Player
{
    Texture2D text_img;
    Vector2 position;

    int nextDirx;
    int nextDiry;

    //player stats
    int maxHealth;
    int currentHealth;
    int attackPoints;
    int deffendPoints;

    bool encounteredSomething;
    std::string whatEncountered;

    bool isActive;

    Player(Texture2D img, int maxHP);

    void Damage(int damage);

    void Encounter(std::string what);

    bool IsDoor(const int (&terrain)[rows][cols]);

    bool IsEnemy(int enemyPosX, int enemyPosY);

    bool IsWall(
        int row,
        int col,
        const int (&terrain)[rows][cols]);

    void Update(
        const int (&terrain)[rows][cols],
        int mapRows,
        int mapCols,
        int tileSize,
        std::vector<Enemy> &enemies);

    void Draw();
};