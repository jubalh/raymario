#include "libraries/physac.h"
#include "libraries/defines.c"

// Structs definitions
typedef enum GameScreen { TITLE, LOADING, GAMEPLAY, END, WIN} GameScreen;

typedef enum Sounds {
	S_SELECT = 0,
	S_JUMP,
	S_KICK,
	S_FIRE,
	S_DIE,
	S_COIN,
	S_TIMELEFT,
	S_GAMEOVER,
	S_COMPLETE_LEVEL, // win
	S_WON,
	S_LAST
} Sounds;

typedef struct Player
{
	Transform transform;
	int lifes;
	bool isDead;
	bool levelCompleted;
	int collectedCoins;
	int timeLeft;

	float moveSpeed;
	float jumpSpeed;
	bool facingRight;
	int frame;
} Player;

typedef struct Game
{
	GameScreen currentScreen;
	bool quitGame;
	bool cheatAllowed;
	bool playingMusic;
	int framesCounter;
	int currentLevel;;
	int levelsCount;
} Game;

// Variables
Player player;
Game game;

int screenWidth;
int screenHeight;

// Sounds variables
Sound sounds[S_LAST];
Music marioSong;

// SpriteFonts variables
SpriteFont font;

// Textures variables
Texture2D background;
Texture2D marioAtlas;
Texture2D environmentAtlas;
Texture2D castleTex;

// Parallax variables
Vector2 lastParallax;
Vector2 parallax;
Vector2 cameraOffset;
Vector2 extraOffset;

// Physics variables
Physics physicsSettings;

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
