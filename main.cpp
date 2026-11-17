#include <raylib.h>
#include <vector>


#include "header/game_config.hpp"
#include "header/player.hpp"
#include "header/enemy.hpp"
#include "src/UserInterface.cpp"

// global variables
std::string outputLog = "Logs:";

// player stats
int playerCurrHealth = 0;
int playerAtkPoints = 0;
int playerDefPoints = 0;

// environments
int terrain[rows][cols] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 9, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 6, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 8},
    {1, 0, 0, 0, 0, 0, 6, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

int next_room[rows][cols] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 8},
    {1, 1, 1, 1, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 1, 1, 1},
    {1, 9, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

// textures
Texture2D player_img;
Texture2D ground_1;
Texture2D wall_1;
Texture2D wall_2;
Texture2D door_1;

// list of enemies
std::vector<Enemy> enemies;

void SetEnvironment(Player &player, std::vector<Enemy> &enemies)
{
    int total_enemies = enemies.size();
    int cur_enemy = 0;

    // set world environment
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            // set the spawn of player
            if ((terrain[row][col] == 9))
            {
                player.position = {(float)col * tileSize, (float)row * tileSize};
            }
            // set the enemies
            else if (terrain[row][col] == 6)
            {
                if (cur_enemy < total_enemies)
                {
                    enemies[cur_enemy].position = {(float)col * tileSize, (float)row * tileSize};
                    cur_enemy++;
                }
            }
        }
    }
}
void DrawEnvironment()
{
    // draw the wall and ground
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            if ((terrain[row][col] == 1))
            {
                DrawTexture(wall_1, col * tileSize, row * tileSize, WHITE);
            }
            else if ((terrain[row][col] == 8))
            {
                DrawTexture(door_1, col * tileSize, row * tileSize, WHITE);
            }
            else
            {
                DrawTexture(ground_1, col * tileSize, row * tileSize, WHITE);
            }
        }
    }
}
void GoToNextRoom(int new_terrain[rows][cols]) {
    //replace the old terrain
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            terrain[row][col] = new_terrain[row][col];
        }
    }
}


int main()
{
    InitWindow(screenWidth, screenHeight, "Tile Game");
    SetTargetFPS(60);
    InitAudioDevice();

    // textures
    ground_1 = LoadTexture("img/ground_06.png");
    wall_1 = LoadTexture("img/block_02.png");
    wall_2 = LoadTexture("img/block_06.png");
    door_1 = LoadTexture("img/crate_07.png");
    player_img = LoadTexture("img/player_05.png");
    // sounds
    Sound sound_fx_1 = LoadSound("snd/can_open_1.mp3");

    // the player
    Player player(player_img, 30);

    // add enemy
    Enemy goblin(player_img, false, 10);
    Enemy wolf(player_img, true, 5);
    enemies.push_back(goblin);
    enemies.push_back(wolf);

    // set the environment
    SetEnvironment(player, enemies);

    while (!WindowShouldClose())
    {
        const float delta = GetFrameTime();

        // Update player
        player.Update(
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
        for (int i = 0; i < enemies.size(); i++)
        {
            if (!enemies[i].isActive)
            {
                // remove enemy
                outputLog = "Enemy is Slayn...";
                PlaySound(sound_fx_1);
                enemies.erase(enemies.begin() + i);
            }
        }

        // check if the player is dead
        if (!player.isActive)
        {
            outputLog = "Player is Dead...";
        }

        //if the player found the door go to the next room
        if(player.IsDoor(terrain)) {
            outputLog = "Player found a door";
            GoToNextRoom(next_room);
            SetEnvironment(player, enemies);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        // the environment
        DrawEnvironment();

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
    UnloadTexture(ground_1);
    UnloadTexture(wall_1);
    UnloadTexture(wall_2);
    UnloadTexture(door_1);
    // unload sounds
    UnloadSound(sound_fx_1);
    CloseWindow();
    return 0;
}