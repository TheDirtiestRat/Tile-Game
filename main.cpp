#include <algorithm>
#include <raylib.h>

#include "header/game_config.hpp"
#include "header/player.hpp"
#include "header/enemy.hpp"
#include "header/UserInterface.hpp"
#include "header/RoomManager.hpp"

// global variables
std::string outputLog = "Logs:";

// player stats
int playerCurrHealth = 0;
int playerAtkPoints = 0;
int playerDefPoints = 0;

// textures
Texture2D player_img;

// list of enemies
std::vector<Enemy> enemies;

int main()
{
    InitWindow(screenWidth, screenHeight, "Tile Game");
    SetTargetFPS(60);
    InitAudioDevice();

    // textures
    LoadRoomTextures();
    player_img = LoadTexture("img/player_05.png");
    // sounds
    Sound sound_fx_1 = LoadSound("snd/can_open_1.mp3");

    // the player
    Player player(player_img, 30);

    // load the starter room
    LoadRoom(0, player, enemies);

    while (!WindowShouldClose())
    {
        const float delta = GetFrameTime();

        // Update player
        player.Update(
            delta,
            terrain,
            rows,
            cols,
            tileSize,
            enemies);

        // set the player variables
        playerCurrHealth = player.currentHealth;
        playerAtkPoints = player.attackPoints;
        playerDefPoints = player.deffendPoints;

        // enemy all update
        for (Enemy &enemy : enemies)
        {
            enemy.Update(
                delta,
                player,
                terrain,
                rows,
                cols,
                tileSize);
        }

        // check what encountered by player
        if (player.encounteredSomething)
        {
            outputLog = player.whatEncountered;
            player.encounteredSomething = false;
        }

        // remove enemies that are not active
        const auto deadEnemiesEnd = std::remove_if(
            enemies.begin(),
            enemies.end(),
            [](const Enemy &enemy)
            {
                return !enemy.isActive && enemy.damageFeedbackTimer <= 0.0f;
            });

        if (deadEnemiesEnd != enemies.end())
        {
            outputLog = "Enemy is Slayn...";
            PlaySound(sound_fx_1);
            enemies.erase(deadEnemiesEnd, enemies.end());
        }

        // check if the player is dead
        if (!player.isActive)
        {
            outputLog = "Player is Dead...";
        }

        // update the room
        RoomUpdate(player, enemies);

        BeginDrawing();
        ClearBackground(BLACK);

        // the environment
        DrawRoomEnvironment();

        // the entities
        player.Draw();

        // draw all enemy
        for (Enemy &enemy : enemies)
        {
            enemy.Draw();
        }

        // the UI
        DrawUI(playerCurrHealth, playerAtkPoints, playerDefPoints, outputLog);

        EndDrawing();
    }
    // unload textures
    UnloadTexture(player_img);
    UnloadRoomTextures();
    // unload sounds
    UnloadSound(sound_fx_1);
    CloseWindow();
    return 0;
}