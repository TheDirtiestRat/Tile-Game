
#include "../header/player.hpp"
#include "../header/enemy.hpp"
#include <raylib.h>

Player::Player(Texture2D img, int maxHP)
{
    text_img = img;
    // position = spawn_location;

    nextDirx = 0;
    nextDiry = 0;

    encounteredSomething = false;
    whatEncountered = "...";

    isActive = true;
    maxHealth = maxHP;
    currentHealth = maxHealth;
    attackPoints = 3;
    deffendPoints = 2;
}



bool Player::IsEnemy(int enemyPosX, int enemyPosY)
{
    return enemyPosX == nextDirx &&
           enemyPosY == nextDiry;
}

bool Player::IsDoor(const int (&terrain)[rows][cols])
{
    if (terrain[nextDiry][nextDirx] == 8) {
        Player::Encounter("That is a door.");
        return true;
    }
    return false;
}

void Player::Encounter(std::string what) {
    encounteredSomething = true;
    whatEncountered = what;
}

void Player::Damage(int damage) {
    currentHealth -= damage;
    Player::Encounter("Player is Attacked!!");

    //if below health then die
    if (currentHealth <= 0) {
        //player is dead
        isActive = false;
    }
}

bool Player::IsWall(
    int row,
    int col,
    const int (&terrain)[rows][cols]
)
{
    if (col < 0 || col >= cols ||
        row < 0 || row >= rows)
    {
        return true;
    }
    if (terrain[row][col] == 1) {
        Player::Encounter("That is a wall");
        return true;
    }

    return false;
}

void Player::Update(
    const int (&terrain)[rows][cols],
    int mapRows,
    int mapCols,
    int tileSize,
    std::vector<Enemy>& enemies
)
{
    Vector2 next_pos = position;

    // Handle input
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
        next_pos.x -= tileSize;

    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
        next_pos.x += tileSize;

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        next_pos.y -= tileSize;

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        next_pos.y += tileSize;

    // Convert pixel position to tile position
    nextDirx = (int)(next_pos.x / tileSize);
    nextDiry = (int)(next_pos.y / tileSize);

    // Check walls
    if (IsWall(nextDiry, nextDirx, terrain))
        return;

    // Check enemies
    for (int i = 0; i < enemies.size(); i++)
    {
        int enemyX = (int)(enemies[i].position.x / tileSize);
        int enemyY = (int)(enemies[i].position.y / tileSize);


        if (IsEnemy(enemyX, enemyY))
        {
            Player::Encounter("Damaging enemy by 2 points!");
            enemies[i].Damage(2);
            return;
        }
    }

    // Move player
    position = next_pos;
}

void Player::Draw()
{
    DrawTexture(
        text_img,
        (int)position.x,
        (int)position.y,
        WHITE
    );
}
