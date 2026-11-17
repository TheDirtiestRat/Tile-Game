#pragma once

#include <raylib.h>

#include "game_config.hpp"

struct Player;

struct Enemy
{
    Texture2D text_img;
    Vector2 position;

    float moveTimer;
    float moveInterval;

    // enemy stats
    int currentHealth;
    int attackPoints;

    float damageFeedbackTimer;
    int lastDamage;

    bool moveRandom;
    bool isActive;

    Enemy(Texture2D img, bool randomMove, int healthPoints);

    void Damage(int damage);

    void AtkPlayer(int damage, Player &player);

    bool IsWall(
        int row,
        int col,
        const int (&terrain)[rows][cols],
        int mapRows,
        int mapCols);

    void Update(
        float dt,
        Player &player,
        const int (&terrain)[rows][cols],
        int mapRows,
        int mapCols,
        int tileSize);

    void Draw();
};