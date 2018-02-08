/*******************************************************************************************
*
*   raylib game - raymario
*
*   This game has been created using raylib 1.7 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*   raylib Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
*   raymario was started by
*   Victor Fisac (www.victorfisac.com)
*
*   Copyright (c) 2015 Victor Fisac (www.victorfisac.com)
*   Copyright (c) 2017 Michael Vetter (jubalh@openmailbox.org)
*
********************************************************************************************/
#include <stdlib.h>
#include <raylib.h>
#include "../globals.h"
#include "../libraries/defines.c"
#include "../level.h"

static int menuSelected = 0;

void TitleUpdate()
{
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed('S'))
    {
        menuSelected = (menuSelected == 0) ? 1 : 0;
        
        // Play navigation sound
        PlaySound(sounds[S_FIRE]);
    }
    else if (IsKeyPressed(KEY_UP) || IsKeyPressed('W'))
    {
        menuSelected = (menuSelected == 1) ? 0 : 1;
        
        // Play navigation sound
        PlaySound(sounds[S_FIRE]);
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        switch (menuSelected)
        {
            case 0:
            {
                // Play select sound
                PlaySound(sounds[S_SELECT]);
                
                game.framesCounter = 0;
                game.currentScreen = LOADING;
                
                LoadLevel(game.currentLevel);
            } break;
            case 1:
            {
                StopMusicStream(marioSong);
                game.framesCounter = 0;
                
                game.quitGame = true;
            } break;
        }
    }
}

void TitleDraw()
{
    BeginDrawing();
    
        ClearBackground(BLACK);
        
        DrawTextEx(font, "ray mario", (Vector2){screenWidth / 2 - MeasureTextEx(font, "ray mario", 60, FONT_SPACING).x / 2, screenHeight * 0.15f}, 60, FONT_SPACING, WHITE);
        
        DrawTextEx(font, ((menuSelected == 0) ? "- Play -" : "Play"), (Vector2){screenWidth / 2 - MeasureTextEx(font, ((menuSelected == 0) ? "- Play -" : "Play"), 40, FONT_SPACING).x / 2, screenHeight * 0.4f}, 40, FONT_SPACING, (menuSelected == 0) ? RED : WHITE);
        
        DrawTextEx(font, ((menuSelected == 1) ? "- Quit -" : "Quit"), (Vector2){screenWidth / 2 - MeasureTextEx(font, ((menuSelected == 1) ? "- Quit -" : "Quit"), 40, FONT_SPACING).x / 2, screenHeight * 0.5f}, 40, FONT_SPACING, (menuSelected == 1) ? RED : WHITE);
    
        // Draw auxiliar floor
        for (int i = 0; i < 80; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                DrawTexturePro(marioAtlas, (Rectangle){FLOOR_OFFSETX, FLOOR_OFFSETY + 3, FLOOR_WIDTH, FLOOR_HEIGHT}, (Rectangle){i * FLOOR_WIDTH * 2.0f, screenHeight * 0.84f + j * FLOOR_HEIGHT * 2.0f, FLOOR_WIDTH * 2.0f, FLOOR_HEIGHT * 2.0f}, (Vector2){0, 0}, 0.0f, WHITE);
            }
        }
        
        // Draw mario
        DrawTexturePro(marioAtlas, (Rectangle){0, 0, MARIO_WIDTH, MARIO_HEIGHT}, (Rectangle){screenWidth / 2 - MARIO_WIDTH / 2 - MARIO_WIDTH, screenHeight * 0.7f, MARIO_WIDTH * 2, MARIO_HEIGHT * 2}, (Vector2){0.5f, 0.5f}, 0.0f, WHITE);
    
        // Draw help message
        DrawTextEx(font, "-SPACE- select    -W- up  -S- down", (Vector2){screenWidth - MeasureTextEx(font, "-SPACE- select    -W- up  -S- down", 15, FONT_SPACING).x, screenHeight * 0.80f}, 15, FONT_SPACING, WHITE);
    
        // Draw credits
        DrawText("Created by: Victor Fisac [www.victorfisac.com]", 5, screenHeight * 0.81f, 10, WHITE);
        
    EndDrawing();
}
