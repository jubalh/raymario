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

// Include Windows libraries
#include <stdlib.h>
#include <stdio.h> // fopen
#include <time.h>

// Include videogame libraries
#include "raylib.h"

// 3rd party
#include "libraries/defines.c"
#include "libraries/physac.h"

// Game
#include "globals.h"
#include "title_screen.h"
#include "loading_screen.h"
#include "end_screen.h"
#include "win_screen.h"
#include "gameplay_screen.h"

// Function declarations
void LoadResources();
void UnloadResources();
void LoadPhysics();
void UpdateDraw();

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    screenWidth = 800;
    screenHeight = 450;
    framesCounter = 0;
    
    srand(time(NULL));
    
#ifndef Debug
    // Show raylib logo
    ShowLogo();
#endif
    
    InitWindow(screenWidth, screenHeight, "raymario v1.0");
    InitAudioDevice();
    
    // Load all resources
    LoadResources();

    // Load physics properties
    LoadPhysics();
    
    // Window initialiation
    quitGame = false;

#ifdef Debug
    cheatAllowed = true;
#else
    cheatAllowed = false;
#endif
    
    // Game initialization
    currentScreen = TITLE;
    currentLevel = 1;
    
    // Menu initialization
    menuSelected = 0;
    
    // Gameplay initialization
    lifes = 3;
    
    // Update
    //--------------------------------------------------------------------------------------
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    
    // Main game loop
    while (!WindowShouldClose() && !quitGame)    // Detect window close button or ESC key
    {
        UpdateDraw();
    }

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
    selectSound = LoadSound("resources/sounds/select.wav");
    playerJump = LoadSound("resources/sounds/player_jump.wav");
    playerKick = LoadSound("resources/sounds/player_kick.wav");
    playerFire = LoadSound("resources/sounds/player_fire.wav");
    playerDie = LoadSound("resources/sounds/player_die.wav");
    coin = LoadSound("resources/sounds/coin.wav");
    leftTime = LoadSound("resources/sounds/left_time.wav");
    gameOver = LoadSound("resources/sounds/gameover.wav");
    win = LoadSound("resources/sounds/win.wav");
    winGame = LoadSound("resources/sounds/wingame.wav");
    marioSong = LoadMusicStream("resources/music/mario_music.ogg");
    
    // Maps initialization
    FILE *imageFile;
    
    for (int i = 0; i < MAX_LEVELS; i++)
    {
        imageFile = fopen(FormatText("resources/maps/level_map_0%i.png", i + 1), "rb");
        
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

void UpdateDraw()
{
    // Global frames counter update
    framesCounter++;
    
    // Music stream logic
    if(playingMusic)
    {
        // Update stream music buffer
        UpdateMusicStream(marioSong);
        
        // Music loop logic
        if(GetMusicTimePlayed(marioSong) >= GetMusicTimeLength(marioSong))
        {
            StopMusicStream(marioSong);
            PlayMusicStream(marioSong);
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
    UnloadSound(selectSound);
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
    StopMusicStream(marioSong);
    playingMusic = false;
}

