#include <raylib.h>
#include <vector>
#include <iostream>

constexpr int tileSize = 64;
constexpr int rows = 7;
constexpr int cols = 8;
constexpr int screenWidth = tileSize * cols;
constexpr int screenHeight = tileSize * (rows + 1);

//environments
int terrain[rows][cols] = {
    {1,1,1,1,1,1,1,1},
    {1,0,1,0,0,0,0,1},
    {1,0,1,0,1,0,0,1},
    {1,0,1,0,1,1,1,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,9,0,0,0,1},
    {1,1,1,1,1,1,1,1},
};

//textures
Texture2D ground_1;
Texture2D wall_1;
Texture2D wall_2;

struct Enemy {
    Texture2D text_img;
    Vector2 position;
    int dirx;
    int diry;
    float wait_timer = 0.0f;
    Enemy(Texture2D img, Vector2 spawn_location) {
        text_img = img;
        position = spawn_location;
    }
    bool IsWall(int row, int col) {
        //pang check kong mi lapas sa window
        if (col < 0 || col >= cols ||
            row < 0 || row >= rows) {
            return true;
        }
        //check kong naay wall
        if(terrain[row][col] == 1) {
            return true;
        }
        return 0;
    }
    void Update(float dt) {
        Vector2 next_pos = position;
        wait_timer += dt;
        if(wait_timer > 2.0f) {
            dirx = GetRandomValue(-1,1);
            diry = GetRandomValue(-1,1);
            wait_timer = 0.0f;

            //handle direction
            if(dirx < 0) next_pos.x -= tileSize;
            else if(dirx > 0) next_pos.x += tileSize;
            else if(diry > 0) next_pos.y -= tileSize;
            else if(diry < 0) next_pos.y += tileSize;
        }

        //position to tile pos
        int nextCol = (int)(next_pos.x / tileSize);
        int nextRow = (int)(next_pos.y / tileSize);

        if(!IsWall(nextRow,nextCol)) {
            position = next_pos;
        }
    }
    void Draw() {
        DrawTexture(text_img,position.x,position.y,RED);
    }
};

//list of enemies
std::vector<Enemy> enemies;

struct Player {
    Texture2D text_img;
    Vector2 position;
    int nextDirx;
    int nextDiry;
    bool canMove;
    Player(Texture2D img, Vector2 spawn_location) {
        text_img = img;
        position = spawn_location;
        canMove = true;
    }
    bool IsEnemy(int enemyPosX, int enemyPosY) {
        //pang check kong mi lapas sa window
        return (enemyPosX == nextDirx && enemyPosY == nextDiry);
    }
    bool IsWall(int row, int col) {
        //pang check kong mi lapas sa window
        if (col < 0 || col >= cols ||
            row < 0 || row >= rows) {
            return true;
        }
        //check kong naay wall
        if(terrain[row][col] == 1) {
            return true;
        }
        return 0;
    }
    void Update() {
        Vector2 next_pos = position;
        //handle input
        if(IsKeyPressed(KEY_LEFT)) next_pos.x -= tileSize;
        if(IsKeyPressed(KEY_RIGHT)) next_pos.x += tileSize;
        if(IsKeyPressed(KEY_UP)) next_pos.y -= tileSize;
        if(IsKeyPressed(KEY_DOWN)) next_pos.y += tileSize;

        //position to tile pos
        nextDirx = (int)(next_pos.x / tileSize);
        nextDiry = (int)(next_pos.y / tileSize);
        
        //check wall
        if(!IsWall(nextDiry,nextDirx)) {
            return;
        }

        //check enemies
        for(Enemy enemy : enemies) {
            int enemyX = (int)enemy.position.x / tileSize;
            int enemyY = (int)enemy.position.y / tileSize;

            std::cout << "Enemy Encounter" << std::endl;

            if(IsEnemy(enemyX, enemyY)) {
                return;
            }
        }        
        //move the player
        position = next_pos;

    }
    void Draw() {
        DrawTexture(text_img,position.x,position.y,WHITE);
    }
};

Vector2 GetSpawnLocation() {
    Vector2 spawn_point = {1.0f,1.0f};
    for(int row = 0; row < rows; row++) {
        for(int col = 0; col < cols; col++) {
            if ((terrain[row][col] == 9)) {
                spawn_point = {(float)col * tileSize, (float)row * tileSize};
            }
        }
    }
    return spawn_point;
}

void DrawEnvironment() {
    //draw the ground
    for(int row = 0; row < rows; row++) {
        for(int col = 0; col < cols; col++) {
            DrawTexture(ground_1, col * tileSize, row * tileSize, WHITE);
        }
    }
    //draw the wall
    for(int row = 0; row < rows; row++) {
        for(int col = 0; col < cols; col++) {
            if ((terrain[row][col] == 1)) {
                DrawTexture(wall_1, col * tileSize, row * tileSize, WHITE);
            }
        }
    }
}

void DrawUI() {
    int font_size = 16;
    float padding = 8.0f;

    DrawText("SCORE: 999", padding,rows * tileSize + padding + 16, font_size, WHITE);
    DrawText("HP: 99", padding,rows * tileSize + padding, font_size, WHITE);
}

int main() {
    
    InitWindow(screenWidth, screenHeight, "Tile Game");
    SetTargetFPS(60);
    InitAudioDevice();

    //textures
    ground_1 = LoadTexture("img/ground_06.png");
    wall_1 = LoadTexture("img\\block_02.png");
    wall_2 = LoadTexture("img\\block_06.png");
    Texture2D player_img = LoadTexture("img/player_05.png");
    //sounds
    Sound sound_fx_1 = LoadSound("snd/can_open_1.mp3");

    //add the player
    Vector2 spawnPoint = GetSpawnLocation();
    Player player(player_img, spawnPoint);
    //add enemy
    Enemy goblin(player_img, {(float)tileSize * 3, (float)tileSize * 2});
    enemies.push_back(goblin);

    while (!WindowShouldClose()) {
        const float delta = GetFrameTime();

        //player update
        player.Update();
        //enemy update;
        goblin.Update(delta);
        
        //tech enemy and player collision
        int ex = (int)(goblin.position.x / tileSize);
        int ey = (int)(goblin.position.y / tileSize);
        if(player.IsEnemy(ex, ey)){
        }

        BeginDrawing();
        ClearBackground(BLACK);

        //debug play sound
        if(IsKeyPressed(KEY_SPACE)) {
            //play sound here
            PlaySound(sound_fx_1);
        }

        //the environment
        DrawEnvironment();

        //the entities
        player.Draw();
        goblin.Draw();

        //the UI
        DrawUI();

        EndDrawing();
    }
    //unload textures
    UnloadTexture(player_img);
    UnloadTexture(ground_1);
    UnloadTexture(wall_1);
    UnloadTexture(wall_2);
    //unload sounds
    UnloadSound(sound_fx_1);
    CloseWindow();
    return 0;
}