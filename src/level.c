#include <stdlib.h>
#include "raylib.h"
#include "globals.h"
#include "libraries/defines.c"
#include "libraries/physac.h"

// Levels are saved as image files (png)
// RGB       - Color  - Representation
// 255,255,0 - Yellow - Floor
// 255,0,0   - Red    - Enemy
// 0,255,0   - Lime   - Coin
// 0,255,255 - Aqua   - Decoration
// 0,0,0     - Black  - Clouds
// 128,0,0   - Maroon - Castle
// 0,128,0   - Green  - Flag

void LoadLevel(int level)
{
    // Parallax initialization
    lastParallax = (Vector2){0, 0};
    parallax = (Vector2){0, 0};
    cameraOffset = (Vector2){0, 0};
    extraOffset = (Vector2){0, 50};
    
    // Player initialization
    player.transform = (Transform){(Vector2){screenWidth * 0.5f - 25, screenHeight * 0.68f}, 0.0f, (Vector2){30, 50}};      
    AddCollider(MAX_BULLETS + 1, (Collider){true, RectangleCollider, (Rectangle){player.transform.position.x, player.transform.position.y, player.transform.scale.x, player.transform.scale.y}, player.transform.scale.x/2 + player.transform.scale.y/2});
    AddRigidbody(MAX_BULLETS + 1, (Rigidbody){true, 2.0f, (Vector2){0, 0}, (Vector2){0, 0}, false, false, true, 0.5f, 0.0f});
    player.isDead = false;
    player.moveSpeed = 5;
    player.jumpSpeed = 20;
    player.facingRight = true;
    player.frame = 0;
    player.collectedCoins = 0;
    player.timeLeft = 100;
    player.levelCompleted = false;
    
    // Bullets initialization
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].position.x = 0.0f;
        bullets[i].position.y = 0.0f;
        bullets[i].rotation = 0.0f;
        bullets[i].scale.x = 10;
        bullets[i].scale.y = 10;
        
        AddCollider(i, (Collider){true, RectangleCollider, (Rectangle){bullets[i].position.x, bullets[i].position.y, bullets[i].scale.x, bullets[i].scale.x}, 0});
        AddRigidbody(i, (Rigidbody){false, 1.0f, (Vector2){0, 0}, (Vector2){0, 0}, false, false, true, 0, 1.0f});
    }
    
    // Map data initialization based on current level
    Color *pixels = GetImageData(LoadImage(FormatText("resources/maps/level_map_%02i.png", level)));
    
    // Level initialization
    usedFloor = 0;
    
    for (int y = 0; y < LEVELIMAGE_HEIGHT; y++)
    {
        for (int x = 0; x < LEVELIMAGE_WIDTH; x++)
        {
            if (pixels[y * LEVELIMAGE_WIDTH + x].r == 255 && pixels[y * LEVELIMAGE_WIDTH + x].g == 255 && pixels[y * LEVELIMAGE_WIDTH + x].b == 0)
            {
                platforms[usedFloor] = (Transform){(Vector2){x * PIXEL_UNIT, y * PIXEL_UNIT}, 0.0f, (Vector2){30, 30}};
                AddCollider(MAX_BULLETS + 2 + usedFloor, (Collider){true, RectangleCollider, (Rectangle){platforms[usedFloor].position.x, platforms[usedFloor].position.y, platforms[usedFloor].scale.x, platforms[usedFloor].scale.y}, 0});
                usedFloor++;
            }
        }
    }
    
    // Enemies initialization
    usedEnemies = 0;
    enemyAngle = 0.0f;
    rangeDistance = 2;
    enemyPadding = 400;
    
    for (int y = 0; y < LEVELIMAGE_HEIGHT; y++)
    {
        for (int x = 0; x < LEVELIMAGE_WIDTH; x++)
        {
            if (pixels[y * LEVELIMAGE_WIDTH + x].r == 255 && pixels[y * LEVELIMAGE_WIDTH + x].g == 0 && pixels[y * LEVELIMAGE_WIDTH + x].b == 0)
            {
                enemies[usedEnemies] = (Transform){(Vector2){x * PIXEL_UNIT, y * PIXEL_UNIT}, 0.0f, (Vector2){30, 30}};
                AddCollider(MAX_BULLETS + 1 + MAX_FLOOR + usedEnemies, (Collider){true, RectangleCollider, (Rectangle){enemies[usedEnemies].position.x, enemies[usedEnemies].position.y, enemies[usedEnemies].scale.x, enemies[usedEnemies].scale.y}, 0});
                AddRigidbody(MAX_BULLETS + 1 + MAX_FLOOR + usedEnemies, (Rigidbody){true, 1.0f, (Vector2){0, 0}, false, false, true, 0.2f, 0.5f});
                usedEnemies++;
            }
        }
    }
    
    // Coins initialization
    usedCoins = 0;
    coinFrame = 0;
    
    for (int y = 0; y < LEVELIMAGE_HEIGHT; y++)
    {
        for (int x = 0; x < LEVELIMAGE_WIDTH; x++)
        {
            if (pixels[y * LEVELIMAGE_WIDTH + x].r == 0 && pixels[y * LEVELIMAGE_WIDTH + x].g == 255 && pixels[y * LEVELIMAGE_WIDTH + x].b == 0)
            {
                coins[usedCoins] = (Transform){(Vector2){x * PIXEL_UNIT, y * PIXEL_UNIT}, 0.0f, (Vector2){20, 30}};
                coinCollected[usedCoins] = false;
                usedCoins++;
            }
        }
    }
    
    // Decoration initialization
    usedDecoration = 0;
    
    for (int i = 0; i < MAX_DECORATION; i++)
    {
        decorationType[i] = rand() % 5;
    }
    
    for (int y = 0; y < LEVELIMAGE_HEIGHT; y++)
    {
        for (int x = 0; x < LEVELIMAGE_WIDTH; x++)
        {
            if (pixels[y * LEVELIMAGE_WIDTH + x].r == 0 && pixels[y * LEVELIMAGE_WIDTH + x].g == 255 && pixels[y * LEVELIMAGE_WIDTH + x].b == 255)
            {
                switch (decorationType[usedDecoration])
                {
                    case 0:
                    {
                        decoration[usedDecoration] = (Transform){(Vector2){x * PIXEL_UNIT * 0.99f, y * PIXEL_UNIT * 1.15f}, 0.0f, (Vector2){64 * 2, 19 * 2}};
                    } break;
                    case 1:
                    {
                        decoration[usedDecoration] = (Transform){(Vector2){x * PIXEL_UNIT * 0.99f, y * PIXEL_UNIT * 1.15f}, 0.0f, (Vector2){48 * 2, 19 * 2}};
                    } break;
                    case 2:
                    {
                        decoration[usedDecoration] = (Transform){(Vector2){x * PIXEL_UNIT * 0.99f, y * PIXEL_UNIT * 1.15f}, 0.0f, (Vector2){119 * 2, 19 * 2}};
                    } break;
                    case 3:
                    {
                        decoration[usedDecoration] = (Transform){(Vector2){x * PIXEL_UNIT * 0.99f, y * PIXEL_UNIT * 1.0f}, 0.0f, (Vector2){16 * 2, 46 * 2}};
                    } break;
                    case 4:
                    {
                        decoration[usedDecoration] = (Transform){(Vector2){x * PIXEL_UNIT * 0.99f, y * PIXEL_UNIT * 1.0f}, 0.0f, (Vector2){64 * 2, 14 * 2}};
                    } break;
                }
                
                usedDecoration++;
            }
        }
    }

    // Clouds initialization
    usedClouds = 0;
    
    for (int y = 0; y < LEVELIMAGE_HEIGHT; y++)
    {
        for (int x = 0; x < LEVELIMAGE_WIDTH; x++)
        {
            if (pixels[y * LEVELIMAGE_WIDTH + x].r == 0 && pixels[y * LEVELIMAGE_WIDTH + x].g == 0 && pixels[y * LEVELIMAGE_WIDTH + x].b == 0)
            {
                clouds[usedClouds] = (Transform){(Vector2){x * PIXEL_UNIT, y * PIXEL_UNIT}, 0.0f, (Vector2){CLOUD_WIDTH * 1.5f, CLOUD_HEIGHT * 1.5f}};
                usedClouds++;
            }
        }
    }
    
    // Castle initialization    
    for (int y = 0; y < LEVELIMAGE_HEIGHT; y++)
    {
        for (int x = 0; x < LEVELIMAGE_WIDTH; x++)
        {
            if (pixels[y * LEVELIMAGE_WIDTH + x].r == 128 && pixels[y * LEVELIMAGE_WIDTH + x].g == 0 && pixels[y * LEVELIMAGE_WIDTH + x].b == 0)
            {
                castle = (Transform){(Vector2){x * PIXEL_UNIT, y * PIXEL_UNIT}, 0.0f, (Vector2){castleTex.width * 1.75f, castleTex.height * 2}};
            }
        }
    }
    
    // Flag properties    
    for (int y = 0; y < LEVELIMAGE_HEIGHT; y++)
    {
        for (int x = 0; x < LEVELIMAGE_WIDTH; x++)
        {
            if (pixels[y * LEVELIMAGE_WIDTH + x].r == 0 && pixels[y * LEVELIMAGE_WIDTH + x].g == 128 && pixels[y * LEVELIMAGE_WIDTH + x].b == 0)
            {
                flag = (Transform){(Vector2){x * PIXEL_UNIT, y * PIXEL_UNIT}, 0.0f, (Vector2){FLAG_WIDTH * 2, FLAG_HEIGHT * 2}};
                AddCollider(MAX_BULLETS + 1 + MAX_FLOOR + MAX_ENEMIES, (Collider){true, RectangleCollider, (Rectangle){flag.position.x, flag.position.y - 230, flag.scale.x, flag.scale.y + 230}, 0});
            }
        }
    }
    
    flagTarget = (Transform){(Vector2){flag.position.x - 16, flag.position.y}, 0.0f, (Vector2){32, 32}};
    flagTargetPos = 230;

    free(pixels);
}
