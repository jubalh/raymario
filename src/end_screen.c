#include "raylib.h"
#include "globals.h"
#include "libraries/defines.c"

void EndUpdate()
{
	if (framesCounter > 240)
	{
		framesCounter = 0;
		currentLevel = 1;
		player.lifes = 3;
		
		currentScreen = TITLE;
	}
}

void EndDraw()
{
	BeginDrawing();
	
		ClearBackground(BLACK);
		
		DrawTextEx(font, "Miss...", (Vector2){screenWidth / 2 - MeasureTextEx(font, "Miss...", 50, FONT_SPACING).x / 2, screenHeight * 0.3f}, 50, FONT_SPACING, RED);
		
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

