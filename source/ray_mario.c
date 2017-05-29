/*******************************************************************************************
*
*   raylib game - ray MARIO
*
*   This game has been created using raylib 1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Victor Fisac (www.victorfisac.com)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

// Include Windows libraries
#include <stdlib.h>
#include <time.h>

// Include videogame libraries
#include "raylib.h"

#ifndef PLATFORM_WEB
    #include "libraries/defines.c"
    #include "libraries/physac.c"
#else
    #include <emscripten/emscripten.h>
#endif

// Structs definitions
typedef enum GameScreen { TITLE, LOADING, GAMEPLAY, END, WIN} GameScreen;

// Global variables
int screenWidth;
int screenHeight;
int framesCounter;
bool quitGame;

// Game variables
GameScreen currentScreen;
int currentLevel;
bool playingMusic;
int mapsCount;

// SpriteFonts variables
SpriteFont font;

// Textures variables
Texture2D background;
Texture2D marioAtlas;
Texture2D environmentAtlas;
Texture2D castleTex;
    
// Sounds variables
Sound select;
Sound playerJump;
Sound playerKick;
Sound playerFire;
Sound playerDie;
Sound coin;
Sound leftTime;
Sound gameOver;
Sound win;
Sound winGame;

// Menu variables
int menuSelected;

// Loading variables
char loadingMessage[10] = "Loading";
int lettersCount;

// Physics variables
Physics physicsSettings;

// Parallax variables
Vector2 lastParallax;
Vector2 parallax;
Vector2 cameraOffset;
Vector2 extraOffset;

// Player variables
Transform player;
int lifes;
bool isDead;
bool completed;
int collectedCoins;
int timeLeft;

float moveSpeed;
float jumpSpeed;
bool facingRight;
int playerFrame;

// Bullets variables
Transform bullets[MAX_BULLETS];
Vector2 lastBullets[MAX_BULLETS];
int samePosition[MAX_BULLETS];

// Level variables
Transform platforms[MAX_FLOOR];
int usedFloor;

// Enemies variables
Transform enemies[MAX_ENEMIES];
int usedEnemies;
int enemiesFrame[MAX_ENEMIES];
float enemyAngle;
int rangeDistance;
int enemyPadding;

// Coins variables
Transform coins[MAX_COINS];
bool coinCollected[MAX_COINS];
int usedCoins;
int coinFrame;

// Decoration variables
Transform decoration[MAX_DECORATION];
int usedDecoration;
int decorationType[MAX_DECORATION];

// Clouds variables
Transform clouds[MAX_CLOUDS];
int usedClouds;

// Castle variables
Transform castle;

// Flag variables
Transform flag;
Transform flagTarget;
int flagTargetPos;

// Function declarations
void LoadResources();
void LoadPhysics();

void LoadLevel(int level);
void UpdateDraw();

void TitleUpdate();
void TitleDraw();

void LoadingUpdate();
void LoadingDraw();

void GameplayUpdate();
void GameplayDraw();

void EndUpdate();
void EndDraw();

void WinUpdate();
void WinDraw();

void UnloadResources();

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    screenWidth = 800;
    screenHeight = 450;
    framesCounter = 0;
    
    srand(time(NULL));
    
    // Show raylib logo
    ShowLogo();
    
    InitWindow(screenWidth, screenHeight, "ray MARIO v1.0 [www.victorfisac.com]");
    InitAudioDevice();
    
    // Load all resources
    LoadResources();

    // Load physics properties
    LoadPhysics();
    
    // Window initialiation
    quitGame = false;
    
    // Game initialization
    currentScreen = TITLE;
    currentLevel = 1;
    
    // Menu initialization
    menuSelected = 0;
    
    // Loading initialization
    lettersCount = 0;
    
    // Gameplay initialization
    lifes = 3;
    
    // Update
    //--------------------------------------------------------------------------------------
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDraw, 0, 1);
#else
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    
    // Main game loop
    while (!WindowShouldClose() && !quitGame)    // Detect window close button or ESC key
    {
        UpdateDraw();
    }
#endif

    // De-Initialization
    //-------------------------------------------------------------------------------------- 
    CloseAudioDevice();
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}

// Function definitions
void LoadResources()
{
    // SpriteFonts loading
    font = LoadSpriteFont("resources/fonts/komika.png");
    
    // Sprites loading
    background = LoadTexture("resources/images/background.jpg");
    marioAtlas = LoadTexture("resources/images/mario_atlas.png");
    environmentAtlas = LoadTexture("resources/images/environment_atlas.png");
    castleTex = LoadTexture("resources/images/castle.png");
    
    // Sounds Loading
    select = LoadSound("resources/sounds/select.wav");
    playerJump = LoadSound("resources/sounds/player_jump.wav");
    playerKick = LoadSound("resources/sounds/player_kick.wav");
    playerFire = LoadSound("resources/sounds/player_fire.wav");
    playerDie = LoadSound("resources/sounds/player_die.wav");
    coin = LoadSound("resources/sounds/coin.wav");
    leftTime = LoadSound("resources/sounds/left_time.wav");
    gameOver = LoadSound("resources/sounds/gameover.wav");
    win = LoadSound("resources/sounds/win.wav");
    winGame = LoadSound("resources/sounds/wingame.wav");
    
    // Maps initialization
    FILE *imageFile;
    
    for (int i = 0; i < MAX_LEVELS; i++)
    {
        imageFile = fopen(FormatText("resources/maps/level_map_0%i.bmp", i + 1), "rb");
        
        if (imageFile == NULL) break;
        
        mapsCount++;
    }
}

void LoadPhysics()
{
    // Physics initialization
    InitPhysics();
    
    physicsSettings = (Physics){true, false, (Vector2){0, -0.98f}};
    SetPhysics(physicsSettings);
}

void LoadLevel(int level)
{
    // Parallax initialization
    lastParallax = (Vector2){0, 0};
    parallax = (Vector2){0, 0};
    cameraOffset = (Vector2){0, 0};
    extraOffset = (Vector2){0, 50};
    
    // Player initialization
    player = (Transform){(Vector2){screenWidth * 0.5f - 25, screenHeight * 0.68f}, 0.0f, (Vector2){30, 50}};      
    AddCollider(MAX_BULLETS + 1, (Collider){true, RectangleCollider, (Rectangle){player.position.x, player.position.y, player.scale.x, player.scale.y}, player.scale.x/2 + player.scale.y/2});
    AddRigidbody(MAX_BULLETS + 1, (Rigidbody){true, 2.0f, (Vector2){0, 0}, (Vector2){0, 0}, false, false, true, 0.5f, 0.0f});
    isDead = false;
    moveSpeed = 5;
    jumpSpeed = 20;
    facingRight = true;
    playerFrame = 0;
    collectedCoins = 0;
    timeLeft = 100;
    completed = false;
    
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
    Color *pixels = GetImageData(LoadImage(FormatText("resources/maps/level_map_%02i.bmp", level)));
    
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

void UpdateDraw()
{
    // Global frames counter update
    framesCounter++;
    
    // Music stream logic
    if(playingMusic)
    {
        // Update stream music buffer
        UpdateMusicStream();
        
        // Music loop logic
        if(GetMusicTimePlayed() >= GetMusicTimeLength())
        {
            StopMusicStream();
            PlayMusicStream(MUSIC_PATH);
        }
    }

    // Update based on game screen
    switch (currentScreen)
    {
        case TITLE:
        {
            TitleUpdate();
            TitleDraw();
        } break;
        case LOADING:
        {
            LoadingUpdate();
            LoadingDraw();
        } break;
        case GAMEPLAY:
        {
            GameplayUpdate();
            GameplayDraw();
        } break;
        case END:
        {
            EndUpdate();
            EndDraw();
        } break;
        case WIN:
        {
            WinUpdate();
            WinDraw();
        } break;
        default: break;
    }
}

void TitleUpdate()
{
#ifndef PLATFORM_WEB
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed('S'))
    {
        menuSelected = (menuSelected == 0) ? 1 : 0;
        
        // Play navigation sound
        PlaySound(playerFire);
    }
    else if (IsKeyPressed(KEY_UP) || IsKeyPressed('W'))
    {
        menuSelected = (menuSelected == 1) ? 0 : 1;
        
        // Play navigation sound
        PlaySound(playerFire);
    }
#endif

    if (IsKeyPressed(KEY_SPACE))
    {
        switch (menuSelected)
        {
            case 0:
            {
                // Play select sound
                PlaySound(select);
                
                framesCounter = 0;
                currentScreen = LOADING;
                
                LoadLevel(currentLevel);
            } break;
            case 1:
            {
                StopMusicStream();
                framesCounter = 0;
                
                quitGame = true;
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
        
    #ifndef PLATFORM_WEB
        DrawTextEx(font, ((menuSelected == 1) ? "- Quit -" : "Quit"), (Vector2){screenWidth / 2 - MeasureTextEx(font, ((menuSelected == 1) ? "- Quit -" : "Quit"), 40, FONT_SPACING).x / 2, screenHeight * 0.5f}, 40, FONT_SPACING, (menuSelected == 1) ? RED : WHITE);
    #endif
    
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
        DrawTextEx(font, "-SPACE- select    -W- up  -S- down", (Vector2){screenWidth - MeasureTextEx(font, "-SPACE- select    -W- up  -S- down", 15, FONT_SPACING).x * 0.35f - 5, screenHeight * 0.80f}, 15, FONT_SPACING, WHITE);
    
        // Draw credits
        DrawText("Created by: Victor Fisac [www.victorfisac.com]", 5, screenHeight * 0.81f, 10, WHITE);
        
    EndDrawing();
}

void LoadingUpdate()
{
    if ((framesCounter % 10) == 0)
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
    
    if (framesCounter > 120)
    {
        currentScreen = GAMEPLAY;
        framesCounter = 0;
        
        if (!playingMusic)
        {
            PlayMusicStream(MUSIC_PATH);
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
        if (!isDead)
        {
            isDead = true;
            SetRigidbodyVelocity(MAX_BULLETS + 1, (Vector2){-5, jumpSpeed * 1.25f});
            SetColliderEnabled(MAX_BULLETS + 1, false);
            
            StopMusicStream();
            playingMusic = false;
            framesCounter = 0;
            
            // Play die sound
            PlaySound(playerDie);
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
                    
                    StopMusicStream();
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
            if (IsKeyDown('W') && GetRigidbody(MAX_BULLETS + 1).isGrounded)
            {
                SetRigidbodyVelocity(MAX_BULLETS + 1, (Vector2){GetRigidbody(MAX_BULLETS + 1).velocity.x, jumpSpeed});
                
                // Play jump sound
                PlaySound(playerJump);
            }
            
            // Check for movement input
            if (IsKeyDown('D') && !GetRigidbody(MAX_BULLETS + 1).isContact)
            {
                facingRight = true;
                SetRigidbodyVelocity(MAX_BULLETS + 1, (Vector2){(IsKeyDown(LEFT_CTRL) ? moveSpeed * 2 : moveSpeed), GetRigidbody(MAX_BULLETS + 1).velocity.y});
                
                if (extraOffset.x > -MAX_OFFSET)
                {
                    extraOffset.x--;
                }
            }
            else if (IsKeyDown('A') && !GetRigidbody(MAX_BULLETS + 1).isContact)
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
                        PlaySound(select);
                        
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
                        
                        StopMusicStream();
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
                StopMusicStream();
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
    
    // Debug mode switch button
    if (IsKeyPressed('P'))
    {
        physicsSettings.debug = !physicsSettings.debug;
        SetPhysics(physicsSettings);
    }
}

void GameplayDraw()
{
    BeginDrawing();

        ClearBackground((Color){94, 145, 255, 255});
        
        if (physicsSettings.debug)
        {
            DrawFPS(10, 10);
            
            // Draw every collider in physics module storage
            for (int i = 0; i < MAX_ELEMENTS; i++)
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

void EndUpdate()
{
    if (framesCounter > 240)
    {
        framesCounter = 0;
        currentLevel = 1;
        lifes = 3;
        
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

void WinUpdate()
{
    if (framesCounter > 240 * 2)
    {
        framesCounter = 0;
        currentLevel = 1;
        lifes = 3;
        
        currentScreen = TITLE;
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

void UnloadResources()
{
    // Unload SpriteFonts
    UnloadSpriteFont(font);
    
    // Unload textures
    UnloadTexture(background);
    UnloadTexture(marioAtlas);
    UnloadTexture(environmentAtlas);
    UnloadTexture(castleTex);
    
    // Unload sounds
    UnloadSound(select);
    UnloadSound(playerJump);
    UnloadSound(playerKick);
    UnloadSound(playerFire);
    UnloadSound(playerDie);
    UnloadSound(coin);
    UnloadSound(leftTime);
    UnloadSound(gameOver);
    UnloadSound(win);
    UnloadSound(winGame);
    
    // Stop music streaming
    StopMusicStream();
    playingMusic = false;
}