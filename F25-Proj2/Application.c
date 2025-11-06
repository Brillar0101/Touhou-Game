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
                drawMenuScreen(g_sContext_p, app->menuSelection);
                app->firstCall = false;
            }

            // BB1 : Move Down

            if (Button_isTapped(&hal->boosterpackS1)) {
                app->menuSelection--;

                // Wrap around: 0 then 2
                if (app->menuSelection < 0) {
                    app->menuSelection = 2;
                }

                // Redrawing menu screen with updated cursors
                drawMenuScreen(g_sContext_p, app->menuSelection);
            }

            // BB2 : Move up

            if (Button_isTapped(&hal->boosterpackS2)) {
                app->menuSelection++;

                // Wrap around: 2 then zero
                if (app->menuSelection > 2) {
                    app->menuSelection = 0;
                }

                // Redrawing the menu screen with updated cursors
                drawMenuScreen(g_sContext_p, app->menuSelection);
            }
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
void drawMenuScreen(Graphics_Context* g_sContext_p,int selection) {
    // Clearing the screen
    Graphics_clearDisplay(g_sContext_p);

    // Setting the font
    Graphics_setFont(g_sContext_p, &g_sFontCmtt12);

    // Drawing menu title
    Graphics_drawString(g_sContext_p, (int8_t*)"MENU", -1, 45, 10, true);

    //"Play" with or without cursor
    if (selection == 0) {
        Graphics_drawString(g_sContext_p, (int8_t*)"> Play", -1, 10, 40, true);
    } else {
        Graphics_drawString(g_sContext_p, (int8_t*)"  Play", -1, 10, 40, true);
    }

    //"Instructions" with or without cursor
    if (selection == 1) {
        Graphics_drawString(g_sContext_p, (int8_t*)"> Instructions", -1, 10, 55, true);
    } else {
        Graphics_drawString(g_sContext_p, (int8_t*)"  Instructions", -1, 10, 55, true);
    }

    //"High Scores" with or without cursor
    if (selection == 2) {
        Graphics_drawString(g_sContext_p, (int8_t*)"> High Scores", -1, 10, 70, true);
    } else {
        Graphics_drawString(g_sContext_p, (int8_t*)"  High Scores", -1, 10, 70, true);
    }
}
