#include "math.h"
#include "raylib.h"
#include "globals.h"
#include "libraries/defines.c"
#include "level.h"

void GameplayUpdate()
{
    // Previous frame data storage
    lastParallax = player.position;
    
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        lastBullets[i] = bullets[i].position;
    }
    
    // Apply physics to bullets
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        ApplyPhysics(i, &bullets[i].position);
    }
    
    // Apply physics to player
    ApplyPhysics(MAX_BULLETS + 1, &player.position);
    
    // Apply physics to enemies
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        ApplyPhysics(MAX_BULLETS + MAX_FLOOR + i + 1, &enemies[i].position);
        
        // Check if enemy is enabled (not dead)
        if (GetCollider(MAX_BULLETS + MAX_FLOOR + i + 1).enabled)
        {
            SetRigidbodyVelocity(MAX_BULLETS + MAX_FLOOR + i + 1, (Vector2){cos(enemyAngle) * rangeDistance, GetRigidbody(MAX_BULLETS + MAX_FLOOR + i + 1).velocity.y});
        }
    }
    
    enemyAngle += 0.01f;
    
    // Bullet logic update
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (GetRigidbody(i).enabled)
        {
            bullets[i].rotation -= 25;
            
            if (bullets[i].position.x + bullets[i].scale.x > screenWidth - cameraOffset.x - extraOffset.x)
            {
                bullets[i].position = (Vector2){0, 0};
                SetRigidbodyVelocity(i, (Vector2){0, 0});
                
                ApplyPhysics(i, &bullets[i].position);
                
                SetRigidbodyEnabled(i, false);
            }
            else if (bullets[i].position.x < 0 - cameraOffset.x - extraOffset.x)
            {
                bullets[i].position = (Vector2){0, 0};
                SetRigidbodyVelocity(i, (Vector2){0, 0});
                
                ApplyPhysics(i, &bullets[i].position);
                
                SetRigidbodyEnabled(i, false);
            }
            else if (bullets[i].position.y < 0 + cameraOffset.y + extraOffset.x)
            {
                bullets[i].position = (Vector2){0, 0};
                SetRigidbodyVelocity(i, (Vector2){0, 0});
                
                ApplyPhysics(i, &bullets[i].position);
                
                SetRigidbodyEnabled(i, false);
            }
            else if (bullets[i].position.y + bullets[i].scale.y > screenWidth + cameraOffset.y + extraOffset.y)
            {
                bullets[i].position = (Vector2){0, 0};
                SetRigidbodyVelocity(i, (Vector2){0, 0});
                
                ApplyPhysics(i, &bullets[i].position);
                
                SetRigidbodyEnabled(i, false);
            }
            else
            {
                for (int j = i + 1; j < MAX_BULLETS; j++)
                {
                    if (CheckCollisionRecs(GetCollider(i).bounds, GetCollider(j).bounds) && GetRigidbody(i).enabled && GetRigidbody(j).enabled)
                    {
                        bullets[i].position = (Vector2){0, 0};
                        SetRigidbodyVelocity(i, (Vector2){0, 0});
                        
                        bullets[j].position = (Vector2){0, 0};
                        SetRigidbodyVelocity(j, (Vector2){0, 0});
                        
                        ApplyPhysics(i, &bullets[i].position);
                        ApplyPhysics(j, &bullets[j].position);
                        
                        SetRigidbodyEnabled(i, false);
                        SetRigidbodyEnabled(j, false);
                    }
                }
            }
        }
    }
    
    // Check player - map bounds collision
    if (player.position.y - player.scale.y > screenHeight - 125)
    {
        if (cheatAllowed)
        {
            // respawn at the top
            player.position.y = 0;
        }
        else
        {
            if (!isDead)
            {
                isDead = true;
                SetRigidbodyVelocity(MAX_BULLETS + 1, (Vector2){-5, jumpSpeed * 1.25f});
                SetColliderEnabled(MAX_BULLETS + 1, false);
                
                StopMusicStream(marioSong);
                playingMusic = false;
                framesCounter = 0;
                
                // Play die sound
                PlaySound(playerDie);
            }
        }
    }
    
    // Player visual frames logic
    if ((framesCounter % (IsKeyDown(LEFT_CTRL) ? 1 : 2)) == 0)
    {
        if (!GetRigidbody(MAX_BULLETS + 1).isGrounded)
        {
            playerFrame = facingRight ? 8 : 9;
        }
        else
        {
            if (GetRigidbody(MAX_BULLETS + 1).velocity.x != 0)
            {
                if (facingRight)
                {
                    if (playerFrame < 3)
                    {
                        playerFrame++;
                    }
                    else
                    {
                        playerFrame = 1;
                    }
                }
                else
                {
                    if (playerFrame < 7)
                    {
                        playerFrame++;
                    }
                    else
                    {
                        playerFrame = 5;
                    }
                }
            }
            else
            {
                playerFrame = facingRight ? 0 : 4;
            }
        }
    }
    
    // Enemies visual frames logic
    if ((framesCounter % 5) == 0)
    {
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (GetCollider(MAX_BULLETS + MAX_FLOOR + i + 1).enabled)
            {
                enemiesFrame[i] = (enemiesFrame[i] > 0) ? 0 : 1;
            }
            else
            {
                enemiesFrame[i] = 2;
            }
        }
    }
    
    // Coins visual frames logic
    if ((framesCounter % 5) == 0)
    {
        coinFrame++;
        
        if (coinFrame > 3)
        {
            coinFrame = 0;
        }
    }
    
    if (!isDead)
    {
        if (!completed)
        {
            // Time challenge logic
            if ((framesCounter % 60 ) == 0)
            {
                timeLeft--;
                
                if (timeLeft == 10)
                {
                    // Play time left warning sound
                    PlaySound(leftTime);
                }
                
                if (timeLeft <= 0)
                {
                    isDead = true;
                    SetRigidbodyVelocity(MAX_BULLETS + 1, (Vector2){-4, jumpSpeed});
                    SetColliderEnabled(MAX_BULLETS + 1, false);
                    
                    StopMusicStream(marioSong);
                    playingMusic = false;
                    framesCounter = 0;
                    
                    // Play die sound
                    PlaySound(playerDie);
                }
            }
            
            // Check for objectives
            if (collectedCoins == usedCoins)
            {
                SetColliderEnabled(MAX_BULLETS + 1 + MAX_FLOOR + MAX_ENEMIES, false);
            }
            
            // Check for jump input
            if ((IsKeyDown(KEY_UP) || IsKeyDown('W')) && GetRigidbody(MAX_BULLETS + 1).isGrounded)
            {
                SetRigidbodyVelocity(MAX_BULLETS + 1, (Vector2){GetRigidbody(MAX_BULLETS + 1).velocity.x, jumpSpeed});
                
                // Play jump sound
                PlaySound(playerJump);
            }
            
            // Check for movement input
            if ((IsKeyDown(KEY_RIGHT) || IsKeyDown('D')) && !GetRigidbody(MAX_BULLETS + 1).isContact)
            {
                facingRight = true;
                SetRigidbodyVelocity(MAX_BULLETS + 1, (Vector2){(IsKeyDown(LEFT_CTRL) ? moveSpeed * 2 : moveSpeed), GetRigidbody(MAX_BULLETS + 1).velocity.y});
                
                if (extraOffset.x > -MAX_OFFSET)
                {
                    extraOffset.x--;
                }
            }
            else if ((IsKeyDown(KEY_LEFT) || IsKeyDown('A')) && !GetRigidbody(MAX_BULLETS + 1).isContact)
            {
                facingRight = false;
                SetRigidbodyVelocity(MAX_BULLETS + 1, (Vector2){(IsKeyDown(LEFT_CTRL) ? -moveSpeed * 2 : -moveSpeed), GetRigidbody(MAX_BULLETS + 1).velocity.y});
                
                if (extraOffset.x < MAX_OFFSET)
                {
                    extraOffset.x++;
                }
            }
            
            // Check for fire input
            if (IsKeyPressed(KEY_SPACE))
            {
                for (int i = 0; i < MAX_BULLETS; i++)
                {
                    if (!GetRigidbody(i).enabled)
                    {
                        bullets[i].position.x = player.position.x + ((facingRight) ? player.scale.x + 10 : -10);
                        bullets[i].position.y = player.position.y + player.scale.y / 2;
                        SetRigidbodyEnabled(i, true);
                        SetRigidbodyVelocity(i, (Vector2){(facingRight) ? 10 : -10, 0});
                        
                        // Play fire sound
                        PlaySound(playerFire);
                        break;
                    }
                }
            }
            
            // Update parallax logic
            parallax = player.position;
            
            cameraOffset.x -= parallax.x - lastParallax.x;
            // cameraOffset.y -= parallax.y - lastParallax.y;
            
            // Check if bullet is stuck during collision (10 frames in same position is considered bullet stuck)
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullets[i].position.x == lastBullets[i].x && bullets[i].position.y == lastBullets[i].y)
                {
                    if (samePosition[i] < 10)
                    {                        
                        samePosition[i]++;
                    }
                    else
                    {
                        bullets[i].position = (Vector2){0, 0};
                        SetRigidbodyVelocity(i, (Vector2){0, 0});
                        
                        ApplyPhysics(i, &bullets[i].position);  
                        
                        SetRigidbodyEnabled(i, false);
                    }
                }
                else
                {
                    samePosition[i] = 0;
                }
            }
        }
        else
        {
            if (flagTargetPos > 0)
            {
                flagTargetPos -= 3;
                SetRigidbodyVelocity(MAX_BULLETS + 1, (Vector2){moveSpeed * 1.07f, GetRigidbody(MAX_BULLETS + 1).velocity.y});
            }
            else
            {
                if (framesCounter > 240 * 1.5f)
                {
                    // Change screen
                    currentScreen = LOADING;
                    framesCounter = 0;
                    
                    currentLevel++;
                    if (currentLevel > mapsCount)
                    {
                        // Play win game sound
                        PlaySound(winGame);
                        
                        currentScreen = WIN;
                    }
                    else
                    {
                        // Play next level sound
                        PlaySound(selectSound);
                        
                        // Load new level map
                        LoadLevel(currentLevel);
                    }
                }
            }
        }
        
        // Check player - enemy collision
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (GetCollider(MAX_BULLETS + 1).enabled && GetCollider(MAX_BULLETS + MAX_FLOOR + i + 1).enabled)
            {
                if (CheckCollisionRecs(GetCollider(MAX_BULLETS + 1).bounds, GetCollider(MAX_BULLETS + MAX_FLOOR + i + 1).bounds))
                {
                    if (GetCollider(MAX_BULLETS + 1).bounds.y + GetCollider(MAX_BULLETS + 1).bounds.height - 10 < GetCollider(MAX_BULLETS + MAX_FLOOR + i + 1).bounds.y)
                    {
                        SetRigidbodyVelocity(MAX_BULLETS + MAX_FLOOR + i + 1, (Vector2){GetRigidbody(i).velocity.x / 2, 10});
                        SetColliderEnabled(MAX_BULLETS + MAX_FLOOR + i + 1, false);
                        
                        SetRigidbodyVelocity(MAX_BULLETS + 1, (Vector2){GetRigidbody(MAX_BULLETS + 1).velocity.x, jumpSpeed / 2});
                        
                        // Play kick sound
                        PlaySound(playerKick);
                    }
                    else
                    {
                        isDead = true;
                        SetRigidbodyVelocity(MAX_BULLETS + 1, (Vector2){GetRigidbody(MAX_BULLETS + MAX_FLOOR + i + 1).velocity.x, jumpSpeed});
                        SetColliderEnabled(MAX_BULLETS + 1, false);
                        
                        StopMusicStream(marioSong);
                        playingMusic = false;
                        framesCounter = 0;
                        
                        // Play die sound
                        PlaySound(playerDie);
                    }
                }
            }
        }
    
        // Check bullets - enemy collision
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            for (int j = 0; j < MAX_ENEMIES; j++)
            {
                if (GetRigidbody(i).enabled && GetCollider(MAX_BULLETS + MAX_FLOOR + j + 1).enabled)
                {
                    if (CheckCollisionRecs(GetCollider(i).bounds, GetCollider(MAX_BULLETS + MAX_FLOOR + j + 1).bounds))
                    {
                        if (GetCollider(MAX_BULLETS + MAX_FLOOR + j + 1).enabled)
                        {
                            SetRigidbodyVelocity(MAX_BULLETS + MAX_FLOOR + j + 1, (Vector2){GetRigidbody(i).velocity.x / 2, 10});
                            SetColliderEnabled(MAX_BULLETS + MAX_FLOOR + j + 1, false);
                            
                            bullets[i].position = (Vector2){0, 0};
                            SetRigidbodyVelocity(i, (Vector2){0, 0});
                            
                            ApplyPhysics(i, &bullets[i].position);
                            
                            SetRigidbodyEnabled(i, false);
                            
                            // Play kick sound
                            PlaySound(playerKick);
                        }
                    }
                }
            }
        }
        
        // Check player - coins collision
        for (int i = 0;  i < MAX_COINS; i++)
        {
            if (!coinCollected[i])
            {
                if (CheckCollisionRecs(GetCollider(MAX_BULLETS + 1).bounds, (Rectangle){coins[i].position.x, coins[i].position.y, coins[i].scale.x, coins[i].scale.y}))
                {
                    collectedCoins++;
                    coinCollected[i] = true;
                    
                    // Play collect coin sound
                    PlaySound(coin);
                }
            }
        }
        
        // Check player - trigger collision
        if (CheckCollisionRecs(GetCollider(MAX_BULLETS + 1).bounds, (Rectangle){flag.position.x, flag.position.y - 300, flag.scale.x, flag.scale.y}))
        {
            if (!completed && collectedCoins == usedCoins)
            {
                completed = true;
                facingRight = true;
                
                framesCounter = 0;
                StopMusicStream(marioSong);
                playingMusic = false;
                
                // Play stage win sound
                PlaySound(win);
            }
        }
    }
    else
    {
        if (framesCounter > 240)
        {
            lifes--;
            
            if (lifes <= 0)
            {
                lifes = 0;
                framesCounter = 0;
                
                // Play game over sound
                PlaySound(gameOver);
                
                currentScreen = END;
            }
            else
            {
                framesCounter = 0;
                
                currentScreen = LOADING;
                
                LoadLevel(currentLevel);
            }
        }
    }
    
#ifdef Debug
    // Debug mode switch button
    if (IsKeyPressed('P'))
    {
        physicsSettings.debug = !physicsSettings.debug;
        SetPhysics(physicsSettings);
    }
#endif
}

void GameplayDraw()
{
    BeginDrawing();

        ClearBackground((Color){94, 145, 255, 255});
        
        if (physicsSettings.debug)
        {
            DrawFPS(10, 10);
            
            // Draw every collider in physics module storage
            for (int i = 0; i < PHYSAC_MAX_ELEMENTS; i++)
            {
                if (GetCollider(i).enabled)
                {
                    if (GetCollider(i).type == RectangleCollider)
                    {
                        DrawRectangleLines(extraOffset.x + cameraOffset.x + GetCollider(i).bounds.x, extraOffset.y + cameraOffset.y + GetCollider(i).bounds.y, GetCollider(i).bounds.width, GetCollider(i).bounds.height, GREEN);
                    }
                    else
                    {
                        DrawCircleLines(extraOffset.x + cameraOffset.x + GetCollider(i).bounds.x, extraOffset.y + cameraOffset.y + GetCollider(i).bounds.y, GetCollider(i).radius, GREEN);
                    }
                }
            }
        }
        else
        {
            // Draw background
            DrawTexturePro(background, (Rectangle){0, 0, background.width, background.height}, (Rectangle){0, 0, screenWidth, screenHeight}, (Vector2){0, 0}, 0.0f, WHITE);
            
            // Draw decoration
            for (int i = 0; i < MAX_DECORATION; i++)
            {
                switch (decorationType[i])
                {
                    case 0:
                    {
                        DrawTexturePro(environmentAtlas, (Rectangle){0, 0, 64, 19}, (Rectangle){extraOffset.x + cameraOffset.x + decoration[i].position.x, decoration[i].position.y, decoration[i].scale.x, decoration[i].scale.y}, (Vector2){0, 0}, 0.0f, WHITE);
                    } break;
                    case 1:
                    {
                        DrawTexturePro(environmentAtlas, (Rectangle){67, 0, 48, 19}, (Rectangle){extraOffset.x + cameraOffset.x + decoration[i].position.x, decoration[i].position.y, decoration[i].scale.x, decoration[i].scale.y}, (Vector2){0, 0}, 0.0f, WHITE);
                    } break;
                    case 2:
                    {
                        DrawTexturePro(environmentAtlas, (Rectangle){0, 0, 118, 19}, (Rectangle){extraOffset.x + cameraOffset.x + decoration[i].position.x, decoration[i].position.y, decoration[i].scale.x, decoration[i].scale.y}, (Vector2){0, 0}, 0.0f, WHITE);
                    } break;
                    case 3:
                    {
                        DrawTexturePro(environmentAtlas, (Rectangle){38, 23, 16, 46}, (Rectangle){extraOffset.x + cameraOffset.x + decoration[i].position.x, decoration[i].position.y, decoration[i].scale.x, decoration[i].scale.y}, (Vector2){0, 0}, 0.0f, WHITE);
                    } break;
                    case 4:
                    {
                        DrawTexturePro(environmentAtlas, (Rectangle){61, 53, 64, 14}, (Rectangle){extraOffset.x + cameraOffset.x + decoration[i].position.x, decoration[i].position.y, decoration[i].scale.x, decoration[i].scale.y}, (Vector2){0, -52}, 0.0f, WHITE);
                    } break;
                }
            }
            
            // Draw clouds
            for (int i = 0; i < MAX_CLOUDS; i++)
            {
                DrawTexturePro(environmentAtlas, (Rectangle){CLOUD_OFFSETX, CLOUD_OFFSETY, CLOUD_WIDTH, CLOUD_HEIGHT}, (Rectangle){extraOffset.x / 4 + cameraOffset.x / 4 + clouds[i].position.x, clouds[i].position.y, clouds[i].scale.x, clouds[i].scale.y}, (Vector2){0, 0}, 0.0f, WHITE);
            }
            
            // Draw castle
            DrawTexturePro(castleTex, (Rectangle){0, 0, 149, castleTex.height}, (Rectangle){extraOffset.x + cameraOffset.x + castle.position.x, extraOffset.y + cameraOffset.y + castle.position.y, castle.scale.x, castle.scale.y}, (Vector2){0, castleTex.height * 1.78f}, 0.0f, WHITE);
            
            
            // Draw flag
            DrawTexturePro(castleTex, (Rectangle){FLAG_OFFSETX, FLAG_OFFSETY, FLAG_WIDTH, FLAG_HEIGHT}, (Rectangle){extraOffset.x + cameraOffset.x + flag.position.x, extraOffset.y + cameraOffset.y + flag.position.y, flag.scale.x, flag.scale.y}, (Vector2){0, FLAG_HEIGHT * 1.78f}, 0.0f, WHITE);
            DrawTexturePro(castleTex, (Rectangle){183, 0, 16, 16}, (Rectangle){extraOffset.x + cameraOffset.x + flagTarget.position.x, flagTarget.position.y - flagTargetPos, flagTarget.scale.x, flagTarget.scale.y}, (Vector2){0, 0}, 0.0f, WHITE);
            
            // Draw floor
            for (int i = 0; i < MAX_FLOOR; i++)
            {
                DrawTexturePro(marioAtlas, (Rectangle){FLOOR_OFFSETX, FLOOR_OFFSETY + 3, FLOOR_WIDTH, FLOOR_HEIGHT}, (Rectangle){extraOffset.x + cameraOffset.x + platforms[i].position.x, extraOffset.y + cameraOffset.y + platforms[i].position.y, platforms[i].scale.x, platforms[i].scale.y}, (Vector2){0, 0}, 0.0f, WHITE);
            }
            
            // Draw player
            if (flagTargetPos > 0) DrawTexturePro(marioAtlas, (Rectangle){MARIO_WIDTH * playerFrame, 0, MARIO_WIDTH, MARIO_HEIGHT}, (Rectangle){extraOffset.x + cameraOffset.x + player.position.x, extraOffset.y + cameraOffset.y + player.position.y, player.scale.x, player.scale.y}, (Vector2){0.5f, 0.5f}, 0.0f, WHITE);
            
            // Draw enemies
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                DrawTexturePro(marioAtlas, (Rectangle){enemiesFrame[i] * ENEMY_WIDTH, MARIO_HEIGHT + 2, ENEMY_WIDTH, ENEMY_HEIGHT}, (Rectangle){extraOffset.x + cameraOffset.x + enemies[i].position.x, extraOffset.y + cameraOffset.y + enemies[i].position.y, enemies[i].scale.x, enemies[i].scale.y}, (Vector2){0, 0}, 0.0f, WHITE);
            }
            
            // Draw bullets
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (GetRigidbody(i).enabled)
                {
                    DrawTexturePro(marioAtlas, (Rectangle){FIREBALL_OFFSETX, FIREBALL_OFFSETY, FIREBALL_WIDTH, FIREBALL_HEIGHT}, (Rectangle){extraOffset.x + cameraOffset.x + bullets[i].position.x, extraOffset.y + cameraOffset.y + bullets[i].position.y, bullets[i].scale.x, bullets[i].scale.y}, (Vector2){bullets[i].scale.x / 2, bullets[i].scale.y / 2}, bullets[i].rotation, WHITE);
                }
            }
            
            // Draw coins
            for (int i = 0; i < MAX_COINS; i++)
            {
                if (!coinCollected[i])
                {
                    DrawTexturePro(marioAtlas, (Rectangle){COIN_OFFSETX + coinFrame * COIN_WIDTH, COIN_OFFSETY, COIN_WIDTH, COIN_HEIGHT}, (Rectangle){extraOffset.x + cameraOffset.x + coins[i].position.x, extraOffset.y + cameraOffset.y + coins[i].position.y, coins[i].scale.x, coins[i].scale.y}, (Vector2){0, 0}, 0.0f, WHITE); 
                }
            }
        }
        
        // Draw gameplay UI
        DrawTextEx(font, FormatText("Coins: %i / %i", collectedCoins, usedCoins), (Vector2){10, 10}, 20, FONT_SPACING, (collectedCoins == usedCoins) ? YELLOW : WHITE);
        DrawTextEx(font, FormatText("       Mario x %i", lifes), (Vector2){150, 10}, 20, FONT_SPACING, WHITE);
        DrawTextEx(font, FormatText("Time: %03i", timeLeft), (Vector2){screenWidth - MeasureTextEx(font, FormatText("Time: %03i", timeLeft), 20, FONT_SPACING).x * 0.5f, 10}, 20, FONT_SPACING, WHITE);
        DrawTextEx(font, FormatText("World: %i", currentLevel), (Vector2){screenWidth / 2, 10}, 20, FONT_SPACING, WHITE);

    EndDrawing();
}
