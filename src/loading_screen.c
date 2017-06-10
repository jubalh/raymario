#include "raylib.h"
#include "globals.h"
#include "libraries/defines.c"

int lettersCount = 0;
char loadingMessage[10] = "Loading";

void LoadingUpdate()
{
    if ((game.framesCounter % 10) == 0)
    {
        lettersCount++;
        
        if (lettersCount > 5)
        {
            lettersCount = 0;
        }
        
        switch (lettersCount)
        {
            case 0: 
            {
                loadingMessage[7] = '\0';
                loadingMessage[8] = '\0';
                loadingMessage[9] = '\0';
                break;
            }
            case 1: 
            {
                loadingMessage[7] = '.';
                loadingMessage[8] = '\0';
                loadingMessage[9] = '\0';
                break;
            }
            case 2: 
            {
                loadingMessage[7] = '.';
                loadingMessage[8] = '.';
                loadingMessage[9] = '\0';
                break;
            }
            case 3: 
            {
                loadingMessage[7] = '.';
                loadingMessage[8] = '.';
                loadingMessage[9] = '.';
                break;
            }
            default: break;
        }
    }
    
    if (game.framesCounter > 120)
    {
        currentScreen = GAMEPLAY;
        game.framesCounter = 0;
        
        if (!playingMusic)
        {
            PlayMusicStream(marioSong);
            playingMusic = true;
        }
    }
}

void LoadingDraw()
{
    BeginDrawing();
    
        ClearBackground(BLACK);
        
        DrawTextEx(font, FormatText("World: %i / %i", currentLevel, mapsCount), (Vector2){screenWidth / 2 - MeasureTextEx(font, FormatText("World: %i / %i", currentLevel, mapsCount), 50, FONT_SPACING).x / 2, screenHeight / 2 - 25}, 50, FONT_SPACING, WHITE);
        
        DrawTextEx(font, loadingMessage, (Vector2){screenWidth - MeasureTextEx(font, "Loading...", 30, FONT_SPACING).x - 20, screenHeight - 30 - 20}, 30, FONT_SPACING, WHITE);
        
    EndDrawing();    
}
