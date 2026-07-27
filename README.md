# Touhou Bullet Hell

A bullet-hell shooter written in bare-metal C for the MSP432P401R, rendered on a
128 x 128 LCD and played with the analog joystick. No operating system and no
game engine: a cooperative main loop, software timers driven off Timer32, and
direct pixel work through TI's grlib.

## Gameplay

Six enemy firing patterns, selected at random and switched every five seconds,
so the player never settles into one dodge rhythm:

| # | Pattern |
|---|---|
| 0 | Vertical stream directly beneath the enemy |
| 1 | Horizontal rake from the left edge |
| 2 | Diagonal, top-left to bottom-right |
| 3 | Diagonal, top-right to bottom-left |
| 4 | Dual horizontal, alternating from both edges |
| 5 | Randomised vertical drops across the play field |

Up to 10 enemy bullets are live at once, drawn from a fixed pool. Bullets are
colour-coded by pattern, so the incoming shape is readable at a glance.

Around the shooting there is a full screen flow: title screen with a timed
auto-transition, menu, instructions, high scores, the game itself, and a game
over screen. High scores persist across rounds. The player has an energy bar
rather than instant death, and both the ship and the enemy are custom sprite
bitmaps rather than grlib primitives.

## Structure

```
F25-Proj2/
  proj2_main.c           Init and main loop
  Application.c/.h       Screen state machine, patterns, collision, rendering
  custom_images.h        Sprite declarations
  player_ship_image.c    Player sprite bitmap
  enemy_alien_image.c    Enemy sprite bitmap
  HAL/
    HAL.c/.h             Hardware abstraction aggregate
    Timer.c/.h           Software timers over Timer32
    Button.c/.h          Debounced button input
    LED.c/.h             LED control
    UART.c/.h            Serial
  msp432p401r.cmd        Linker command file
```

The HAL layer is the part worth looking at. Application code never touches a
peripheral register directly; it asks the HAL for debounced button state and for
expired software timers. That is what makes a cooperative main loop workable
without an RTOS, because nothing in the loop is allowed to block.

## Design notes

**Fixed pools, no allocation.** `MAX_ENEMY_BULLETS` is 10 and the bullet array is
static. There is no heap on this target and no allocation in the loop, so the
worst-case frame cost is bounded.

**Software timers instead of delays.** Pattern switching, bullet spawning, and
screen transitions each own a `SWTimer`. The loop polls for expiry rather than
blocking, so input stays responsive while several things run on their own
schedules.

**Collision is axis-aligned bounding box.** Bullet-to-enemy and bullet-to-player
both reduce to rectangle overlap, which is what fits the cycle budget at this
frame rate.

## Building

Built with Code Composer Studio for the MSP432P401R LaunchPad with the
Educational BoosterPack MKII. Import `F25-Proj2` as a CCS project, connect the
board, and flash.

Requires TI's `grlib` graphics library and the MSP432 driver library.

## Hardware

- MSP432P401R LaunchPad
- Educational BoosterPack MKII: 128 x 128 colour LCD, analog joystick, buttons
