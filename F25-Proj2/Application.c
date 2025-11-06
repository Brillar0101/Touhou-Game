/*
 * Author : Barakaeli Lawuo
 * Date : 2/11/25
 * Application.c file for Project 2
 */

#include <Application.h>

// Set initial values
Application Application_construct() {
    Application app;
    app.hasDrawnTitle = false;
    return app;
}


void Application_loop(Application* app, HAL* hal, Graphics_Context* g_sContext_p) {
    if (!app->hasDrawnTitle) {
        drawTitleScreen(g_sContext_p);
        app->hasDrawnTitle = true;
    }

    // Refreshing buttons for non-blocking checks
    HAL_refresh(hal);
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
