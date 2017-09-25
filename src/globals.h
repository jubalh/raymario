#include "libraries/physac.h"
#include "libraries/defines.c"

// Structs definitions
typedef enum GameScreen {
	TITLE = 0, 
	LOADING, 
	GAMEPLAY, 
	END, 
	WIN
} GameScreen;

typedef enum Sounds {
	S_SELECT = 0,
	S_JUMP,
	S_KICK,
	S_FIRE,
	S_DIE,
	S_COIN,
	S_TIMELEFT,
	S_GAMEOVER,
	S_COMPLETE_LEVEL,
	S_WON,
	S_LAST
} Sounds;

typedef struct Player
{
	//TODO Transform transform;
	PhysicsBody transform;
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

// Bullets variables
PhysicsBody bullets[MAX_BULLETS];
Vector2 lastBullets[MAX_BULLETS];
int samePosition[MAX_BULLETS];

// Level variables
PhysicsBody platforms[MAX_FLOOR];
int usedFloor;

// Enemies variables
PhysicsBody enemies[MAX_ENEMIES];
int usedEnemies;
int enemiesFrame[MAX_ENEMIES];
float enemyAngle;
int rangeDistance;
int enemyPadding;

// Coins variables
PhysicsBody coins[MAX_COINS];
bool coinCollected[MAX_COINS];
int usedCoins;
int coinFrame;

// Decoration variables
PhysicsBody decoration[MAX_DECORATION];
int usedDecoration;
int decorationType[MAX_DECORATION];

// Clouds variables
PhysicsBody clouds[MAX_CLOUDS];
int usedClouds;

// Castle variables
PhysicsBody castle;

// Flag variables
PhysicsBody flag;
PhysicsBody flagTarget;
int flagTargetPos;
