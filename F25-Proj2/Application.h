/*
 *
 * Author : Barakaeli Lawuo
 * Date : 2/11/25
 * Application.h
 * HW9
 * Implemented features so far
 *       - Title Screen
 *       - Auto transition after 3 seconds
 *       - Cursor Movement with wrapping
 *       - Added a Player on Screen
 *       - Added enemy on Screen
 *       - Added Game Over Screen
 *       - Saving the High Score
 *       - Using Joystick for Player Movement
 *       - Three Bullet Patterns implemented
 *       - Different colored bullets
 *       - Use of energy bars
 *       - Added custom images
 *
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_


#include <HAL/HAL.h>
#include <ti/grlib/grlib.h>
#include <HAL/Timer.h>


#define MAX_ENEMY_BULLETS 10

// Enum for screen states
enum _ScreenState {
    TITLE_SCREEN,
    MENU_SCREEN,
    INSTRUCTIONS_SCREEN,
    HIGH_SCORES_SCREEN,
    GAME_SCREEN,
    GAME_OVER_SCREEN

};
typedef enum _ScreenState ScreenState;

// Bullet Struct
struct _Bullet {
    int x;
    int y;
    bool active;
    int patternID;
};
typedef struct _Bullet Bullet;

// Enemy Bullet Struct

struct _EnemyBulletSystem {
    Bullet bullets[MAX_ENEMY_BULLETS];
    int currentPattern;
    SWTimer patternTimer;
    SWTimer spawnTimer;
};
typedef struct _EnemyBulletSystem EnemyBulletSystem;

// Player struct
struct _Player {
    int x;
    int y;
    int health;
    Bullet bullet;
};
typedef struct _Player Player;

// Enemy Struct
struct _Enemy {
    int x;
    int y;
    int health;
    int directionX;
    SWTimer moveTimer;
};
typedef struct _Enemy Enemy;

// Power up Struct
struct _PowerUp {
    int x;
    int y;
    bool active;
};
typedef struct _PowerUp PowerUp;


// Struct showing what is show on the screen
struct _Application {

    ScreenState currentScreen;
    bool firstCall;
    SWTimer titleTimer;
    bool timerStarted;
    int menuSelection;
    int highScores[5];

    Player player;
    int gameTime;
    Enemy enemy;
    EnemyBulletSystem enemyBullets;

    int finalScore;
    bool playerWon;

    PowerUp powerUp;
    SWTimer powerUpSpawnTimer;
};
typedef struct _Application Application;

// Initializing  the app
Application Application_construct();

// Main loop
void Application_loop(Application* app, HAL* hal, Graphics_Context* g_sContext_p);

// Drawing functions
void drawTitleScreen(Graphics_Context* g_sContext_p);
void drawMenuScreen(Graphics_Context* g_sContext_p, int selection);
void drawInstructionsScreen(Graphics_Context* g_sContext_p);
void drawHighScoresScreen(Graphics_Context* g_sContext_p, Application* app_p);
void drawGameScreen(Graphics_Context* g_sContext_p, Application* app_p);

// Game Helper functions
void initializeGame(Application* app);
void updatePlayerPosition(Player* player, int joystickX, int joystickY);

       // Enemy bullet system
bool checkBulletEnemyCollision(Bullet* bullet, Enemy* enemy);
void initEnemyBullets(EnemyBulletSystem* system);
void updateEnemyBullets(Application* app);
void spawnEnemyBullet(EnemyBulletSystem* system, Enemy* enemy);
bool checkEnemyBulletPlayerCollision(Bullet* bullet, Player* player);

#endif /* APPLICATION_H_ */
