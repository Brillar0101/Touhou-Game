/*
 *
 * Author : Barakaeli Lawuo
 * Date : 2/11/25
 * Application.h
 * HW9
 * Implemented features so far
 *       - Title Screen
 *
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <HAL/HAL.h>
#include <ti/grlib/grlib.h>

// Struct showing what is show on the screen
struct _Application {
    bool hasDrawnTitle;
};
typedef struct _Application Application;

// Initializing  the app
Application Application_construct();

// Main loop
void Application_loop(Application* app, HAL* hal, Graphics_Context* g_sContext_p);

// Drawing the title screen
void drawTitleScreen(Graphics_Context* g_sContext_p);

#endif /* APPLICATION_H_ */
