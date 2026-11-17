
#include "../header/player.hpp"
#include "../header/enemy.hpp"
#include <algorithm>
#include <cmath>
#include <raylib.h>

namespace
{
constexpr float damageFeedbackDuration = 0.45f;
constexpr int damageTextSize = 20;
}

Player::Player(Texture2D img, int maxHP)
    : stats(maxHP)
{
    text_img = img;
    // position = spawn_location;

    nextDirx = 0;
    nextDiry = 0;

    encounteredSomething = false;
    whatEncountered = "...";

    isActive = true;
    damageFeedbackTimer = 0.0f;
    lastDamage = 0;
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
    if (damage <= 0 || !isActive)
        return;

    const int appliedDamage = stats.TakeDamage(damage);
    if (appliedDamage <= 0)
        return;

    damageFeedbackTimer = damageFeedbackDuration;
    lastDamage = appliedDamage;
    Player::Encounter(TextFormat("Player takes %d damage!", appliedDamage));

    if (stats.GetCurrentHealth() <= 0) {
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
    float dt,
    const int (&terrain)[rows][cols],
    int mapRows,
    int mapCols,
    int tileSize,
    std::vector<Enemy>& enemies
)
{
    damageFeedbackTimer = std::max(0.0f, damageFeedbackTimer - dt);

    if (!isActive)
        return;

    Vector2 next_pos = position;
    bool moved = false;

    // Handle input
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
        next_pos.x -= tileSize;
        moved = true;
    }

    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        next_pos.x += tileSize;
        moved = true;
    }

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        next_pos.y -= tileSize;
        moved = true;
    }

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        next_pos.y += tileSize;
        moved = true;
    }

    if (!moved)
        return;

    // Convert pixel position to tile position
    nextDirx = static_cast<int>(next_pos.x / tileSize);
    nextDiry = static_cast<int>(next_pos.y / tileSize);

    // Check walls and doors
    if (IsWall(nextDiry, nextDirx, terrain))
        return;

    // Check enemies
    for (int i = 0; i < enemies.size(); i++)
    {
        if (!enemies[i].isActive)
            continue;

        int enemyX = static_cast<int>(enemies[i].position.x / tileSize);
        int enemyY = static_cast<int>(enemies[i].position.y / tileSize);

        if (IsEnemy(enemyX, enemyY))
        {
            const int damage = stats.GetAttackPoints();
            Player::Encounter(TextFormat("Damaging enemy by %d points!", damage));
            enemies[i].Damage(damage);
            return;
        }
    }

    // Move player
    position = next_pos;
}

void Player::Draw()
{
    Vector2 drawPosition = position;
    Color tint = WHITE;

    if (damageFeedbackTimer > 0.0f)
    {
        const float elapsed = damageFeedbackDuration - damageFeedbackTimer;
        const float progress = elapsed / damageFeedbackDuration;
        drawPosition.x += std::sin(elapsed * 80.0f) * 4.0f;

        if (static_cast<int>(elapsed / 0.07f) % 2 == 0)
            tint = RED;

        const char* damageText = TextFormat("-%d", lastDamage);
        const int textWidth = MeasureText(damageText, damageTextSize);
        DrawText(
            damageText,
            static_cast<int>(position.x + (tileSize - textWidth) / 2),
            static_cast<int>(position.y - 20.0f - progress * 28.0f),
            damageTextSize,
            Fade(RED, 1.0f - progress));
    }

    DrawTexture(
        text_img,
        static_cast<int>(drawPosition.x),
        static_cast<int>(drawPosition.y),
        tint
    );
}
