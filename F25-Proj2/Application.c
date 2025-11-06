/*
 * Author : Barakaeli Lawuo
 * Date : 2/11/25
 * Application.c file for Project 2
 */

#include <Application.h>

// Set initial values
Application Application_construct() {
    Application app;

    // Starting application on title screen
    app.currentScreen = TITLE_SCREEN;
    app.firstCall = true;

    // Creating a 3 second timer
    app.titleTimer = SWTimer_construct(3000);
    app.timerStarted = false;

    return app;
}


void Application_loop(Application* app, HAL* hal, Graphics_Context* g_sContext_p) {

    // Refreshing buttons for non-blocking checks
    HAL_refresh(hal);


    if (app->currentScreen == TITLE_SCREEN) {

            // Draws title of the screen o first call
            if (app->firstCall) {
                drawTitleScreen(g_sContext_p);

                // Then starts a 3 seconds timer
                SWTimer_start(&app->titleTimer);
                app->timerStarted = true;

                app->firstCall = false;
            }

            // Checks if 3 seconds has passed
            if (app->timerStarted && SWTimer_expired(&app->titleTimer)) {
                // Moves to Menu Screen
                app->currentScreen = MENU_SCREEN;
                app->firstCall = true;
            }
        }

        else if (app->currentScreen == MENU_SCREEN) {

            // Drawing the menu on the first call
            if (app->firstCall) {
                drawMenuScreen(g_sContext_p);
                app->firstCall = false;
            }

            // Going to add buttons functionality later
        }
}

// Drawing the title screen

void drawTitleScreen(Graphics_Context* g_sContext_p) {
    // Clearing the screen
    Graphics_clearDisplay(g_sContext_p);

    // Setting the font
    Graphics_setFont(g_sContext_p, &g_sFontCmtt12);

    // Title page info
    Graphics_drawString(g_sContext_p, (int8_t*)"ECE 2564", -1, 30, 30, true);
    Graphics_drawString(g_sContext_p, (int8_t*)"Project 2", -1, 30, 45, true);
    Graphics_drawString(g_sContext_p, (int8_t*)"Touhou", -1, 40, 70, true);

    // The name of the student
    Graphics_drawString(g_sContext_p, (int8_t*)"Baraka Lawuo", -1, 20, 95, true);
}


// Drawing the menu screen
void drawMenuScreen(Graphics_Context* g_sContext_p) {
    // Clearing the screen
    Graphics_clearDisplay(g_sContext_p);

    // Setting the font
    Graphics_setFont(g_sContext_p, &g_sFontCmtt12);

    // Drawing menu title
    Graphics_drawString(g_sContext_p, (int8_t*)"MENU", -1, 45, 10, true);

    // Drawing menu options with curosr on the first option ( Will fix this later)
    Graphics_drawString(g_sContext_p, (int8_t*)"> Play", -1, 10, 40, true);
    Graphics_drawString(g_sContext_p, (int8_t*)"  Instructions", -1, 10, 55, true);
    Graphics_drawString(g_sContext_p, (int8_t*)"  High Scores", -1, 10, 70, true);
}
