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
#include "screens/title_screen.h"
#include "screens/loading_screen.h"
#include "screens/end_screen.h"
#include "screens/win_screen.h"
#include "screens/gameplay_screen.h"

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
    game.framesCounter = 0;
    
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
    game.quitGame = false;

#ifdef Debug
    game.cheatAllowed = true;
#else
    game.cheatAllowed = false;
#endif
    
    // Game initialization
    game.currentScreen = TITLE;
    game.currentLevel = 1;
    
    // Gameplay initialization
    player.lifes = 3;
    
    // Update
    //--------------------------------------------------------------------------------------
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    
    // Main game loop
    while (!WindowShouldClose() && !game.quitGame)    // Detect window close button or ESC key
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
    background = LoadTexture("resources/images/background.png");
    marioAtlas = LoadTexture("resources/images/mario_atlas.png");
    environmentAtlas = LoadTexture("resources/images/environment_atlas.png");
    castleTex = LoadTexture("resources/images/castle.png");
    
    // Sounds Loading
    sounds[S_SELECT] = LoadSound("resources/sounds/select.wav");
    sounds[S_JUMP] = LoadSound("resources/sounds/player_jump.wav");
    sounds[S_KICK] = LoadSound("resources/sounds/player_kick.wav");
    sounds[S_FIRE] = LoadSound("resources/sounds/player_fire.wav");
    sounds[S_DIE] = LoadSound("resources/sounds/player_die.wav");
    sounds[S_COIN] = LoadSound("resources/sounds/coin.wav");
    sounds[S_TIMELEFT] = LoadSound("resources/sounds/left_time.wav");
    sounds[S_GAMEOVER] = LoadSound("resources/sounds/gameover.wav");
    sounds[S_COMPLETE_LEVEL] = LoadSound("resources/sounds/win.wav");
    sounds[S_WON] = LoadSound("resources/sounds/wingame.wav");
    marioSong = LoadMusicStream("resources/music/mario_music.ogg");
    
    // Maps initialization
    FILE *imageFile;
    
    for (int i = 0; i < MAX_LEVELS; i++)
    {
        imageFile = fopen(FormatText("resources/maps/level_map_0%i.png", i + 1), "rb");
        
        if (imageFile == NULL) break;
        
        game.levelsCount++;
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
    game.framesCounter++;
    
    // Music stream logic
    if(game.playingMusic)
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
    switch (game.currentScreen)
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
    UnloadSound(sounds[S_SELECT]);
    UnloadSound(sounds[S_JUMP]);
    UnloadSound(sounds[S_KICK]);
    UnloadSound(sounds[S_FIRE]);
    UnloadSound(sounds[S_DIE]);
    UnloadSound(sounds[S_COIN]);
    UnloadSound(sounds[S_TIMELEFT]);
    UnloadSound(sounds[S_GAMEOVER]);
    UnloadSound(sounds[S_COMPLETE_LEVEL]);
    UnloadSound(sounds[S_WON]);
    
    // Stop music streaming
    StopMusicStream(marioSong);
    game.playingMusic = false;
}

