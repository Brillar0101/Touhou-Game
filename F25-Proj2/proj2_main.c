/**
 * Starter code for Fall 2025 Project 2. Good luck!
 *
 * We recommend copy/pasting your HAL folder from Project 1
 * into this project.
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include <Application.h>
#include <HAL/HAL.h>
#include <HAL/Timer.h>
#include "HAL/LcdDriver/Crystalfontz128x128_ST7735.h"

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void InitNonBlockingLED() {
  GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
}

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void PollNonBlockingLED() {
  GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
  if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == 0) {
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
  }
}

// Initializing graphics
void InitGraphics(Graphics_Context* g_sContext_p) {
    // Setring up LCD
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);


    Graphics_initContext(g_sContext_p,
                         &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);

     // Setting colors
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);

    // Setting font
    Graphics_setFont(g_sContext_p, &g_sFontCmtt12);

    Graphics_clearDisplay(g_sContext_p);
}

int main() {
  WDT_A_holdTimer();

  // Initializing system timing
  InitSystemTiming();

  // Do not remove this line. This is your non-blocking check.
  InitNonBlockingLED();

  // Setting up HAL
  HAL hal = HAL_construct();

  // Setting up the Application
  Application app = Application_construct();

  // Setting up the graphics
  Graphics_Context g_sContext;
  InitGraphics(&g_sContext);

  while (1) {
    // Do not remove this line. This is your non-blocking check.
    PollNonBlockingLED();

    // Running the game
    Application_loop(&app, &hal, &g_sContext);
  }
}
