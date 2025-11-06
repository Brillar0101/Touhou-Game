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
 *
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_


#include <HAL/HAL.h>
#include <ti/grlib/grlib.h>
#include <HAL/Timer.h>

// Enum for screen states
enum _ScreenState {
    TITLE_SCREEN,
    MENU_SCREEN,
    INSTRUCTIONS_SCREEN,
    HIGH_SCORES_SCREEN,
    GAME_SCREEN

};
typedef enum _ScreenState ScreenState;

// Player struct
struct _Player {
    int x;
    int y;
    int health;
};
typedef struct _Player Player;


// Struct showing what is show on the screen
struct _Application {

    ScreenState currentScreen;   // Which screen user is on
    bool firstCall;              // First time drawing on screen ?
    SWTimer titleTimer;          // 3 sec timer
    bool timerStarted;           // Timer started yet ?
    int menuSelection;           // Menu selection
    int highScores[5];

    Player player;
    int gameTime;
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

#endif /* APPLICATION_H_ */
