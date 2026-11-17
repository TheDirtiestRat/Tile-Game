
#pragma once

#include <raylib.h>
#include <string>
#include <sstream>

inline void DrawTextWrapped(
    const char* text,
    Rectangle box,
    int fontSize,
    int spacing,
    Color color
)
{
    std::stringstream stream(text);
    std::string word;
    std::string line;

    float y = box.y;

    while (stream >> word)
    {
        std::string testLine = line.empty()
            ? word
            : line + " " + word;

        float textWidth = MeasureText(testLine.c_str(), fontSize);

        if (textWidth > box.width && !line.empty())
        {
            if (y + fontSize > box.y + box.height)
                return;

            DrawText(line.c_str(), box.x, y, fontSize, color);

            y += fontSize + spacing;
            line = word;
        }
        else
        {
            line = testLine;
        }
    }

    if (!line.empty() && y + fontSize <= box.y + box.height)
    {
        DrawText(line.c_str(), box.x, y, fontSize, color);
    }
}
