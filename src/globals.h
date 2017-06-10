#include "libraries/physac.h"
#include "libraries/defines.c"

// Structs definitions
typedef enum GameScreen { TITLE, LOADING, GAMEPLAY, END, WIN} GameScreen;

typedef struct Physics {
	bool debug;
} Physics;

// Global variables
int framesCounter;
bool quitGame;
bool cheatAllowed;

// Menu variables
int menuSelected;
int screenWidth;
int screenHeight;

// Sounds variables
Sound selectSound;
Sound playerJump;
Sound playerKick;
Sound playerFire;
Sound playerDie;
Sound coin;
Sound leftTime;
Sound gameOver;
Sound win;
Sound winGame;
Music marioSong;

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

// Parallax variables
Vector2 lastParallax;
Vector2 parallax;
Vector2 cameraOffset;
Vector2 extraOffset;

// Player variables
PhysicsBody player;
int lifes;
bool isDead;
bool completed;
int collectedCoins;
int timeLeft;

float moveSpeed;
float jumpSpeed;
bool facingRight;
int playerFrame;

// Physics variables
Physics physicsSettings;

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
