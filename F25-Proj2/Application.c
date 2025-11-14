/*
 * Author : Barakaeli Lawuo
 * Date : 2/11/25
 * Application.c file for Project 2
 */

#include <Application.h>
#include <stdio.h>

// Constants for the game
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128
#define MARGIN_LEFT 20
#define MARGIN_RIGHT 20
#define PLAYER_SIZE 8
#define PLAYER_STARTING_HEALTH 25
#define BULLET_SIZE 2
#define BULLET_SPEED 3
#define ENEMY_SIZE 10
#define ENEMY_STARTING_HEALTH 25
#define PLAYER_DAMAGE 5
#define ENEMY_DAMAGE 5
#define ENEMY_BULLET_SPEED 2
#define PATTERN_SWITCH_TIME 5000
#define BULLET_SPAWN_TIME 1000

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

    // Initializing game variables
    app.gameTime = 0;

    // Initializing the player
    app.player.x = SCREEN_WIDTH / 2;
    app.player.y = SCREEN_HEIGHT - 30;
    app.player.health = PLAYER_STARTING_HEALTH;

    // Initializing the bullet position
    app.player.bullet.active = false;
    app.player.bullet.x = 0;
    app.player.bullet.y = 0;

    // Initializing the enemy
    app.enemy.x = SCREEN_WIDTH / 2;
    app.enemy.y = 25;
    app.enemy.health = ENEMY_STARTING_HEALTH;

    // Initializing enemy bullet system
    initEnemyBullets(&app.enemyBullets);

    // Intializing a timer
    initEnemyBullets(&app.enemyBullets);

    // Initialize game over variables
    app.finalScore = 0;
    app.playerWon = false;

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

                    initializeGame(app);
                    app->currentScreen = GAME_SCREEN;
                    app->firstCall = true;

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

        // Logic for game screen

        else if (app->currentScreen == GAME_SCREEN) {

            if (app->firstCall) {
                drawGameScreen(g_sContext_p, app);
                app->firstCall = false;
            }


            //Increment game time
            app->gameTime++;

            // Using Joystick for directions
            int joystickX = Joystick_getXDirection(&hal->joystick);
            int joystickY = Joystick_getYDirection(&hal->joystick);

            // Updating the position of the player
            if (joystickX != 0 || joystickY != 0) {
                updatePlayerPosition(&app->player, joystickX, joystickY);
            }

            // BB1 - Firing a bullet if a bullet is not active
            if (Button_isTapped(&hal->boosterpackS1)) {
                if (!app->player.bullet.active) {
                    // Firing a new bullet
                    app->player.bullet.active = true;
                    app->player.bullet.x = app->player.x;
                    app->player.bullet.y = app->player.y - PLAYER_SIZE;
                }
            }

            // Updates the bullet position
            if (app->player.bullet.active) {
                app->player.bullet.y -= BULLET_SPEED;

                // Deactivates if bullet goes off the screen
                if (app->player.bullet.y < 0) {
                    app->player.bullet.active = false;
                }
            }

            // Checks collision with the enemy
            if (checkBulletEnemyCollision(&app->player.bullet, &app->enemy)) {
                // Hits and damage the enemy then deactivates bullet
                app->enemy.health -= PLAYER_DAMAGE;
                app->player.bullet.active = false;

                // Prevents negative health
                if (app->enemy.health < 0) {
                    app->enemy.health = 0;
                }
            }

            //Updates enemy bullets
            updateEnemyBullets(app);

            // Checks for game over conditions

            if (app->player.health <= 0) {
                // Game lost
                app->playerWon = false;
                app->finalScore = 0;
                app->currentScreen = GAME_OVER_SCREEN;
                app->firstCall = true;
            }
            else if (app->enemy.health <= 0) {
                // Game won
                app->playerWon = true;
                // Calculates the score
                app->finalScore = (app->gameTime / 100) + (app->player.health * 10);

                // Saves the score to high score
                updateHighScores(app, app->finalScore);

                app->currentScreen = GAME_OVER_SCREEN;
                app->firstCall = true;
            }


            // Redraw the screen again
            drawGameScreen(g_sContext_p, app);
        }

        // Game over Logic

        else if (app->currentScreen == GAME_OVER_SCREEN) {

            if (app->firstCall) {
                drawGameOverScreen(g_sContext_p, app);
                app->firstCall = false;
            }

            // JSB - Returns to Menu
            if (Button_isTapped(&hal->boosterpackJS)) {
                app->currentScreen = MENU_SCREEN;
                app->firstCall = true;
            }
        }

}


// Initializing the game when it starts

void initializeGame(Application* app) {
    // Resets player position
    app->player.x = SCREEN_WIDTH / 2;
    app->player.y = SCREEN_HEIGHT - 30;
    app->player.health = PLAYER_STARTING_HEALTH;

    // Resets game
    app->gameTime = 0;

    app->player.bullet.active = false;
    app->player.bullet.x = 0;
    app->player.bullet.y = 0;

    app->enemy.x = SCREEN_WIDTH / 2;
    app->enemy.y = 25;
    app->enemy.health = ENEMY_STARTING_HEALTH;

    initEnemyBullets(&app->enemyBullets);

}

void updatePlayerPosition(Player* player, int joystickX, int joystickY) {
    // Player's moving speed
    player->x += joystickX * 2;
    player->y += joystickY * 2;

    // Keeps the player within x-axis margins
    if (player->x < MARGIN_LEFT + PLAYER_SIZE) {
        player->x = MARGIN_LEFT + PLAYER_SIZE;
    }
    if (player->x > SCREEN_WIDTH - MARGIN_RIGHT - PLAYER_SIZE) {
        player->x = SCREEN_WIDTH - MARGIN_RIGHT - PLAYER_SIZE;
    }

    // Keeps the player within the y-axis margins
    if (player->y < PLAYER_SIZE) {
        player->y = PLAYER_SIZE;
    }
    if (player->y > SCREEN_HEIGHT - PLAYER_SIZE) {
        player->y = SCREEN_HEIGHT - PLAYER_SIZE;
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

// Enemy health decreases
bool checkBulletEnemyCollision(Bullet* bullet, Enemy* enemy) {
    if (!bullet->active) {
        return false;
    }

    // Calculates distance between bullet and enemy
    int dx = bullet->x - enemy->x;
    int dy = bullet->y - enemy->y;
    int distanceSquared = dx * dx + dy * dy;

    // Hits enemy only if the distance is less than bullet + enemy size
    int hitRange = BULLET_SIZE + ENEMY_SIZE;

    return (distanceSquared < hitRange * hitRange);
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


// Drawing the game screen

void drawGameScreen(Graphics_Context* g_sContext_p, Application* app_p) {
    Graphics_clearDisplay(g_sContext_p);

    Graphics_setFont(g_sContext_p, &g_sFontCmtt12);

    //Draws timer
    char timeStr[20];
    sprintf(timeStr, "Time: %d", app_p->gameTime / 100);
    Graphics_drawString(g_sContext_p, (int8_t*)timeStr, -1, 5, 5, true);

    // Draws the player

    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
    Graphics_fillCircle(g_sContext_p, app_p->player.x, app_p->player.y, PLAYER_SIZE);

    // Draws the enemy
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_RED);
    Graphics_fillRectangle(g_sContext_p, &(Graphics_Rectangle){
        app_p->enemy.x - ENEMY_SIZE/2,
        app_p->enemy.y - ENEMY_SIZE/2,
        app_p->enemy.x + ENEMY_SIZE/2,
        app_p->enemy.y + ENEMY_SIZE/2
    });

    // Draws a bullet if active
    if (app_p->player.bullet.active) {
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_CYAN);
        Graphics_fillCircle(g_sContext_p, app_p->player.bullet.x,
                           app_p->player.bullet.y, BULLET_SIZE);
    }
    // Draws enemy's bullet
    int i;
    for (i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (app_p->enemyBullets.bullets[i].active) {
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_RED);
            Graphics_fillCircle(g_sContext_p,
                               app_p->enemyBullets.bullets[i].x,
                               app_p->enemyBullets.bullets[i].y,
                               BULLET_SIZE);
        }
    }

    // Draws a health circle

    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
    Graphics_drawCircle(g_sContext_p, 15, 113, 12);

    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    char healthStr[10];
    sprintf(healthStr, "%d", app_p->player.health);
    Graphics_drawString(g_sContext_p, (int8_t*)healthStr, -1, 8, 108, true);

    // Draws enemy's health
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_RED);
    Graphics_drawCircle(g_sContext_p, 113, 15, 12);

    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    char enemyHealthStr[10];
    sprintf(enemyHealthStr, "%d", app_p->enemy.health);
    Graphics_drawString(g_sContext_p, (int8_t*)enemyHealthStr, -1, 100, 10, true);

    // Resets the color
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
}

// Draws the game over Screen
void drawGameOverScreen(Graphics_Context* g_sContext_p, Application* app_p) {
    Graphics_clearDisplay(g_sContext_p);
    Graphics_setFont(g_sContext_p, &g_sFontCmtt12);

    // Shows if player won or lost
    if (app_p->playerWon) {
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_GREEN);
        Graphics_drawString(g_sContext_p, (int8_t*)"You Win!", -1, 35, 35, true);
    } else {
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_RED);
        Graphics_drawString(g_sContext_p, (int8_t*)"You Lose", -1, 35, 35, true);
    }

    // Shows the score
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    char scoreStr[30];
    sprintf(scoreStr, "Score: %d", app_p->finalScore);
    Graphics_drawString(g_sContext_p, (int8_t*)scoreStr, -1, 30, 60, true);

    // Shows the time
    char timeStr[30];
    sprintf(timeStr, "Time: %d sec", app_p->gameTime / 100);
    Graphics_drawString(g_sContext_p, (int8_t*)timeStr, -1, 25, 75, true);

    // Navigation instruction
    Graphics_drawString(g_sContext_p, (int8_t*)"Press JSB to", -1, 20, 100, true);
    Graphics_drawString(g_sContext_p, (int8_t*)"continue", -1, 35, 113, true);
}


// Initializes the enemy bullet system
void initEnemyBullets(EnemyBulletSystem* system) {
    int i;
    for (i = 0; i < MAX_ENEMY_BULLETS; i++) {
        system->bullets[i].active = false;
        system->bullets[i].x = 0;
        system->bullets[i].y = 0;
    }

    system->currentPattern = 0;
    system->patternTimer = SWTimer_construct(PATTERN_SWITCH_TIME);
    system->spawnTimer = SWTimer_construct(BULLET_SPAWN_TIME);

    SWTimer_start(&system->patternTimer);
    SWTimer_start(&system->spawnTimer);
}


// Spawns a new enemy bullet
void spawnEnemyBullet(EnemyBulletSystem* system, Enemy* enemy) {
    int i;
    for (i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (!system->bullets[i].active) {
            system->bullets[i].active = true;
            system->bullets[i].patternID = system->currentPattern;

            // Vertical pattern
            if (system->currentPattern == 0) {
                system->bullets[i].x = enemy->x;
                system->bullets[i].y = enemy->y + ENEMY_SIZE;
            }
            // Horizontal pattern
            else {
                system->bullets[i].x = MARGIN_LEFT;
                system->bullets[i].y = enemy->y + 20 + (i * 10);
            }

            break;
        }
    }
}

// Updates the enemy bullets

void updateEnemyBullets(Application* app) {
    // Checks if ready to switch patterns
    if (SWTimer_expired(&app->enemyBullets.patternTimer)) {
        app->enemyBullets.currentPattern = (app->enemyBullets.currentPattern + 1) % 2;
        SWTimer_start(&app->enemyBullets.patternTimer);
    }

    // Checks if it is time to spawn a new bullet
    if (SWTimer_expired(&app->enemyBullets.spawnTimer)) {
        spawnEnemyBullet(&app->enemyBullets, &app->enemy);
        SWTimer_start(&app->enemyBullets.spawnTimer);
    }

    // Updates  all active bullets
    int i;
    for (i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (app->enemyBullets.bullets[i].active) {

            // Pattern 0: Vertical
            if (app->enemyBullets.bullets[i].patternID == 0) {
                app->enemyBullets.bullets[i].y += ENEMY_BULLET_SPEED;
            }
            // Pattern 1: Horizontal
            else {
                app->enemyBullets.bullets[i].x += ENEMY_BULLET_SPEED;
            }

            if (checkEnemyBulletPlayerCollision(&app->enemyBullets.bullets[i], &app->player)) {
                // Hit player
                app->player.health -= ENEMY_DAMAGE;
                app->enemyBullets.bullets[i].active = false;

                // Health always greater than zero
                if (app->player.health < 0) {
                    app->player.health = 0;
                }
            }

            // Deactivates bullet if off the screen
            if (app->enemyBullets.bullets[i].y > SCREEN_HEIGHT ||
                app->enemyBullets.bullets[i].x > SCREEN_WIDTH) {
                app->enemyBullets.bullets[i].active = false;
            }
        }
    }
}


// Checks if the enemy hits the player
bool checkEnemyBulletPlayerCollision(Bullet* bullet, Player* player) {

    // checks only if bullet is active
    if (!bullet->active) {
        return false;
    }

    // Calculate distance between bullet and player
    int dx = bullet->x - player->x;
    int dy = bullet->y - player->y;
    int distanceSquared = dx * dx + dy * dy;

    // Bullet hits the player
    int hitRange = BULLET_SIZE + PLAYER_SIZE;

    return (distanceSquared < hitRange * hitRange);
}

// Updates the high score
void updateHighScores(Application* app_p, int newScore) {
    int i, j;
    int insertPosition = -1;

    // Checks the order of the score
    for (i = 0; i < 5; i++) {
        if (newScore > app_p->highScores[i]) {
            insertPosition = i;
            break;
        }
    }
    // Does not add the score if it did not make the top 5
    if (insertPosition == -1) {
        return;
    }
    for (j = 4; j > insertPosition; j--) {
        app_p->highScores[j] = app_p->highScores[j - 1];
    }

    // Adds a new score
    app_p->highScores[insertPosition] = newScore;
}
