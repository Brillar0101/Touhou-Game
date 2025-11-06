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

    // Begin with Menu Screen
    app.menuSelection = 0;

    // Initiliazing the high score to zero
    int i;
    for (i = 0; i < 5; i++) {
        app.highScores[i] = 0;
    }

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

            // JSB : Selecting the current option

            if (Button_isTapped(&hal->boosterpackJS)) {
                if (app->menuSelection == 0) {
                }
                else if (app->menuSelection == 1) {  // Instruction Select

                    app->currentScreen = INSTRUCTIONS_SCREEN;
                    app->firstCall = true;
                }
                else if (app->menuSelection == 2) {  // High Score Select

                    app->currentScreen = HIGH_SCORES_SCREEN;
                    app->firstCall = true;
                }
            }
        }

        // Logic for instruction screen
        else if (app->currentScreen == INSTRUCTIONS_SCREEN) {

            if (app->firstCall) {
                drawInstructionsScreen(g_sContext_p);
                app->firstCall = false;
            }

            if (Button_isTapped(&hal->boosterpackS2)) {  // Return to Menu
                app->currentScreen = MENU_SCREEN;
                app->firstCall = true;
            }
        }

        // Logic for high scores screen
        else if (app->currentScreen == HIGH_SCORES_SCREEN) {

            if (app->firstCall) {
                drawHighScoresScreen(g_sContext_p, app);
                app->firstCall = false;
            }

            if (Button_isTapped(&hal->boosterpackS2)) {  // Return to Menu
                app->currentScreen = MENU_SCREEN;
                app->firstCall = true;
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


// Drawing instructions screen
void drawInstructionsScreen(Graphics_Context* g_sContext_p) {
    Graphics_clearDisplay(g_sContext_p);
    Graphics_setFont(g_sContext_p, &g_sFontCmtt12);

    // Title of the screen
    Graphics_drawString(g_sContext_p, (int8_t*)"Instructions", -1, 20, 5, true);

    // Game instructions
    Graphics_drawString(g_sContext_p, (int8_t*)"Move character", -1, 5, 25, true);
    Graphics_drawString(g_sContext_p, (int8_t*)"to avoid being", -1, 5, 37, true);
    Graphics_drawString(g_sContext_p, (int8_t*)"hit by bullets.", -1, 5, 49, true);

    Graphics_drawString(g_sContext_p, (int8_t*)"Press BB1 to", -1, 5, 61, true);
    Graphics_drawString(g_sContext_p, (int8_t*)"fire at enemy.", -1, 5, 73, true);

    Graphics_drawString(g_sContext_p, (int8_t*)"First one to", -1, 5, 85, true);
    Graphics_drawString(g_sContext_p, (int8_t*)"run out loses.", -1, 5, 97, true);

    // Navigation instructions
    Graphics_drawString(g_sContext_p, (int8_t*)"BB2: Go Back", -1, 8, 115, true);
}


// Drawing high scores screen
void drawHighScoresScreen(Graphics_Context* g_sContext_p, Application* app_p) {
    Graphics_clearDisplay(g_sContext_p);
    Graphics_setFont(g_sContext_p, &g_sFontCmtt12);

    // Title of the screen
    Graphics_drawString(g_sContext_p, (int8_t*)"High Scores", -1, 25, 10, true);

    // Displaying 5 scores
    char scoreStr[20];
    int i;
    int yPos = 35;

    for (i = 0; i < 5; i++) {
        sprintf(scoreStr, "%d. %d", i + 1, app_p->highScores[i]);
        Graphics_drawString(g_sContext_p, (int8_t*)scoreStr, -1, 40, yPos, true);
        yPos += 15;
    }

    // Navigation instruction
    Graphics_drawString(g_sContext_p, (int8_t*)"BB2: Go Back", -1, 8, 118, true);
}
