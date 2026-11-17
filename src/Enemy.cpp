
#include "../header/player.hpp"
#include "../header/enemy.hpp"

#include <queue>
#include <vector>

Enemy::Enemy(Texture2D img, bool randomMove, int healthPoints)
{
    text_img = img;
    // position = spawn_location;

    moveTimer = 0.0f;
    moveInterval = 2.0f;

    moveRandom = randomMove;
    isActive = true;
    currentHealth = healthPoints;
    // enemy stats
    attackPoints = 2;
}

void Enemy::AtkPlayer(int damage, Player &player)
{
    player.Damage(damage);
}

void Enemy::Damage(int damage)
{
    currentHealth -= damage;

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

        int nextCol = (int)(nextPosition.x / tileSize);
        int nextRow = (int)(nextPosition.y / tileSize);

        // Check wall
        if (IsWall(nextRow, nextCol, terrain, mapRows, mapCols))
            return;

        // Check player position
        int playerCol = (int)(player.position.x / tileSize);
        int playerRow = (int)(player.position.y / tileSize);

        if (nextCol == playerCol && nextRow == playerRow)
        {
            // damage the player here
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

        int enemyCol = (int)(position.x / tileSize);
        int enemyRow = (int)(position.y / tileSize);
        int playerCol = (int)(player.position.x / tileSize);
        int playerRow = (int)(player.position.y / tileSize);

        // Already next to the player on the same tile.
        if (enemyCol == playerCol && enemyRow == playerRow)
        {
            // Damage the player here.
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

        // Store the previous tile so we can rebuild the path.
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

                // Allow the player tile as the destination.
                if (!(nextRow == playerRow && nextCol == playerCol) &&
                    IsWall(nextRow, nextCol, terrain, mapRows, mapCols))
                    continue;

                visited[nextRow][nextCol] = true;
                previous[nextRow][nextCol] = current;
                open.push({nextRow, nextCol});
            }
        }

        if (!foundPlayer)
            return; // No path to the player.

        // Walk backward from the player to find the first tile in the path.
        Node step = {playerRow, playerCol};

        while (previous[step.row][step.col].row != enemyRow ||
               previous[step.row][step.col].col != enemyCol)
        {
            step = previous[step.row][step.col];

            if (step.row < 0)
                return;
        }

        // The next tile is the player's tile.
        if (step.row == playerRow && step.col == playerCol)
        {
            // Damage the player here.
            AtkPlayer(attackPoints, player);
            return;
        }

        position = {
            (float)(step.col * tileSize),
            (float)(step.row * tileSize)};
    }
}

void Enemy::Draw()
{
    DrawTexture(
        text_img,
        (int)position.x,
        (int)position.y,
        RED);
}
