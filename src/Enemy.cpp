
#include "../header/player.hpp"
#include "../header/enemy.hpp"

#include <algorithm>
#include <cmath>
#include <queue>
#include <vector>

namespace
{
constexpr float damageFeedbackDuration = 0.45f;
constexpr int damageTextSize = 20;
}

Enemy::Enemy(Texture2D img, bool randomMove, int healthPoints)
{
    text_img = img;
    // position = spawn_location;

    moveTimer = 0.0f;
    moveInterval = 2.0f;

    moveRandom = randomMove;
    isActive = true;
    currentHealth = healthPoints;
    damageFeedbackTimer = 0.0f;
    lastDamage = 0;
    // enemy stats
    attackPoints = 2;
}

void Enemy::AtkPlayer(int damage, Player &player)
{
    player.Damage(damage);
}

void Enemy::Damage(int damage)
{
    if (damage <= 0 || !isActive)
        return;

    currentHealth = std::max(0, currentHealth - damage);
    damageFeedbackTimer = damageFeedbackDuration;
    lastDamage = damage;

    // if below health then die
    if (currentHealth <= 0)
    {
        isActive = false;
    }
}

bool Enemy::IsWall(
    int row,
    int col,
    const int (&terrain)[rows][cols],
    int mapRows,
    int mapCols)
{
    if (row < 0 || row >= mapRows ||
        col < 0 || col >= mapCols)
    {
        return true;
    }

    return terrain[row][col] == 1;
}

void Enemy::Update(
    float dt,
    Player &player,
    const int (&terrain)[rows][cols],
    int mapRows,
    int mapCols,
    int tileSize)
{
    damageFeedbackTimer = std::max(0.0f, damageFeedbackTimer - dt);

    if (!isActive || !player.isActive)
        return;

    if (moveRandom)
    {
        moveTimer += dt;

        if (moveTimer < moveInterval)
            return;

        moveTimer = 0.0f;

        // Random movement direction
        int dirX = 0;
        int dirY = 0;

        int direction = GetRandomValue(0, 3);

        if (direction == 0)
            dirX = -1;
        if (direction == 1)
            dirX = 1;
        if (direction == 2)
            dirY = -1;
        if (direction == 3)
            dirY = 1;

        // Calculate next position
        Vector2 nextPosition = {
            position.x + dirX * tileSize,
            position.y + dirY * tileSize};

        int nextCol = static_cast<int>(nextPosition.x / tileSize);
        int nextRow = static_cast<int>(nextPosition.y / tileSize);

        // Check wall
        if (IsWall(nextRow, nextCol, terrain, mapRows, mapCols))
            return;

        // Check player position
        int playerCol = static_cast<int>(player.position.x / tileSize);
        int playerRow = static_cast<int>(player.position.y / tileSize);

        if (nextCol == playerCol && nextRow == playerRow)
        {
            AtkPlayer(attackPoints, player);
            return;
        }

        // Move enemy
        position = nextPosition;
    }
    else
    {
        moveTimer += dt;

        if (moveTimer < moveInterval)
            return;

        moveTimer = 0.0f;

        const int enemyCol = static_cast<int>(position.x / tileSize);
        const int enemyRow = static_cast<int>(position.y / tileSize);
        const int playerCol = static_cast<int>(player.position.x / tileSize);
        const int playerRow = static_cast<int>(player.position.y / tileSize);

        if (enemyCol == playerCol && enemyRow == playerRow)
        {
            AtkPlayer(attackPoints, player);
            return;
        }

        struct Node
        {
            int row;
            int col;
        };

        std::queue<Node> open;
        std::vector<std::vector<bool>> visited(
            mapRows, std::vector<bool>(mapCols, false));

        std::vector<std::vector<Node>> previous(
            mapRows, std::vector<Node>(mapCols, {-1, -1}));

        open.push({enemyRow, enemyCol});
        visited[enemyRow][enemyCol] = true;

        const int rowStep[] = {-1, 1, 0, 0};
        const int colStep[] = {0, 0, -1, 1};

        bool foundPlayer = false;

        while (!open.empty())
        {
            Node current = open.front();
            open.pop();

            if (current.row == playerRow && current.col == playerCol)
            {
                foundPlayer = true;
                break;
            }

            for (int i = 0; i < 4; ++i)
            {
                int nextRow = current.row + rowStep[i];
                int nextCol = current.col + colStep[i];

                if (nextRow < 0 || nextRow >= mapRows ||
                    nextCol < 0 || nextCol >= mapCols)
                    continue;

                if (visited[nextRow][nextCol])
                    continue;

                if (!(nextRow == playerRow && nextCol == playerCol) &&
                    IsWall(nextRow, nextCol, terrain, mapRows, mapCols))
                    continue;

                visited[nextRow][nextCol] = true;
                previous[nextRow][nextCol] = current;
                open.push({nextRow, nextCol});
            }
        }

        if (!foundPlayer)
            return;

        std::vector<Node> path;
        Node current = {playerRow, playerCol};
        path.push_back(current);

        while (previous[current.row][current.col].row >= 0 &&
               previous[current.row][current.col].col >= 0)
        {
            current = previous[current.row][current.col];
            path.push_back(current);

            if (current.row == enemyRow && current.col == enemyCol)
                break;
        }

        if (path.size() < 2)
            return;

        const Node nextStep = path[path.size() - 2];
        const int nextTileX = nextStep.col * tileSize;
        const int nextTileY = nextStep.row * tileSize;

        if (nextStep.row == playerRow && nextStep.col == playerCol)
        {
            AtkPlayer(attackPoints, player);
            return;
        }

        position = {static_cast<float>(nextTileX), static_cast<float>(nextTileY)};
    }
}

void Enemy::Draw()
{
    if (!isActive && damageFeedbackTimer <= 0.0f)
        return;

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
        tint);
}
