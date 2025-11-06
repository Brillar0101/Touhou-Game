/*
 *
 * Author : Barakaeli Lawuo
 * Date : 2/11/25
 * Application.h
 * HW9
 * Implemented features so far
 *       - Title Screen
 *       - Auto transition after 3 seconds
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
    MENU_SCREEN
};
typedef enum _ScreenState ScreenState;


// Struct showing what is show on the screen
struct _Application {

    ScreenState currentScreen;   // Which screen user is on
    bool firstCall;              // First time drawing on screen ?
    SWTimer titleTimer;          // 3 sec timer
    bool timerStarted;           // Timer started yet ?
};
typedef struct _Application Application;

// Initializing  the app
Application Application_construct();

// Main loop
void Application_loop(Application* app, HAL* hal, Graphics_Context* g_sContext_p);

// Drawing functions
void drawTitleScreen(Graphics_Context* g_sContext_p);
void drawMenuScreen(Graphics_Context* g_sContext_p);

#endif /* APPLICATION_H_ */
