#include "raylib.h"
#include "globals.h"
#include "libraries/defines.c"

void WinUpdate()
{
    if (game.framesCounter > 240 * 2)
    {
        game.framesCounter = 0;
        game.currentLevel = 1;
        player.lifes = 3;

        game.currentScreen = TITLE;
    }
}

void WinDraw()
{
    BeginDrawing();

        ClearBackground(BLACK);

        DrawTextEx(font, "You saved the princess!", (Vector2){screenWidth / 2 - MeasureTextEx(font, "You saved the princess!", 50, FONT_SPACING).x / 2, screenHeight * 0.35f}, 50, FONT_SPACING, YELLOW);

        // Draw mario
        DrawTexturePro(marioAtlas, (Rectangle){0, 0, MARIO_WIDTH, MARIO_HEIGHT}, (Rectangle){screenWidth / 2 - MARIO_WIDTH / 2 - 50, screenHeight * 0.7f, MARIO_WIDTH * 2, MARIO_HEIGHT * 2}, (Vector2){0.5f, 0.5f}, 0.0f, WHITE);

        // Draw princess
        DrawTexturePro(environmentAtlas, (Rectangle){PRINCESS_OFFSETX, PRINCESS_OFFSETY, PRINCESS_WIDTH, PRINCESS_HEIGHT}, (Rectangle){screenWidth / 2 - MARIO_WIDTH / 2 + 50, screenHeight * 0.7f, PRINCESS_WIDTH * 2.25f, PRINCESS_HEIGHT * 2.25f}, (Vector2){0.5f, -7}, 0.0f, WHITE);

        // Draw auxiliar floor
        for (int i = 0; i < 80; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                DrawTexturePro(marioAtlas, (Rectangle){FLOOR_OFFSETX, FLOOR_OFFSETY + 3, FLOOR_WIDTH, FLOOR_HEIGHT}, (Rectangle){i * FLOOR_WIDTH * 2.0f, screenHeight * 0.84f + j * FLOOR_HEIGHT * 2.0f, FLOOR_WIDTH * 2.0f, FLOOR_HEIGHT * 2.0f}, (Vector2){0, 0}, 0.0f, WHITE);
            }
        }

    EndDrawing();
}

