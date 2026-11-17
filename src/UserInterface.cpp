#pragma once

#include <raylib.h>
#include <string>
#include "../header/game_config.hpp"
#include "../header/text_utils.hpp"


void DrawUI(int playerCurrHealth, int playerAtkPoints, int playerDefPoints, std::string outputLog)
{
    int font_size = 16;
    float padding = 8.0f;

    // Stats UI
    DrawText(TextFormat("HP: %d", playerCurrHealth), padding, rows * tileSize + padding, font_size, WHITE);
    DrawText(TextFormat("ATK: %d", playerAtkPoints), padding, rows * tileSize + padding + 16, font_size, WHITE);
    DrawText(TextFormat("DEF: %d", playerDefPoints), padding, rows * tileSize + padding + (16 * 2), font_size, WHITE);

    // Logs box
    Rectangle logBox = {
        padding + (tileSize * 4),
        rows * tileSize + padding,
        240,
        60 - (12)};

    DrawRectangleRec(logBox, DARKGRAY);
    DrawRectangleLinesEx(logBox, 1, WHITE);

    DrawTextWrapped(
        outputLog.c_str(),
        {logBox.x + 5,
         logBox.y + 5,
         logBox.width - 10,
         logBox.height - 10},
        font_size,
        2,
        WHITE);
}