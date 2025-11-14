/*
 *  Joystick.h
 *
 *  Hardware Abstraction Layer for Joystick using ADC
 *  Author : Barakaeli Lawuo
 *  Date : November 13th 2025
 *
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

 // Joystick Thresholds

#define JOYSTICK_CENTER 8192
#define JOYSTICK_DEADZONE 3000
#define JOYSTICK_LEFT_THRESHOLD 3000
#define JOYSTICK_RIGHT_THRESHOLD 13000
#define JOYSTICK_UP_THRESHOLD 3000
#define JOYSTICK_DOWN_THRESHOLD 13000

// Joystick states
enum _JoystickTapState {
    NOT_TAPPED,
    TAPPED
};
typedef enum _JoystickTapState JoystickTapState;

// Joystick struct

struct _Joystick {
    uint16_t x;
    uint16_t y;

    // FSM for tap detection states
    JoystickTapState tapLeftState;
    JoystickTapState tapRightState;
    JoystickTapState tapUpState;
    JoystickTapState tapDownState;

    // Tap detection outouts
    bool isTappedLeft;
    bool isTappedRight;
    bool isTappedUp;
    bool isTappedDown;
};
typedef struct _Joystick Joystick;

// Constructor to initialize the Joystick and ADC
Joystick Joystick_construct();

// Reads ADC and updates x,y and tap states
void Joystick_refresh(Joystick* joystick_p);

// Checks the direction where the Joystick is pressed
bool Joystick_isPressedLeft(Joystick* joystick_p);
bool Joystick_isPressedRight(Joystick* joystick_p);
bool Joystick_isPressedUp(Joystick* joystick_p);
bool Joystick_isPressedDown(Joystick* joystick_p);

// Checks if the Joystick was tapped in a certain direction
bool Joystick_isTappedLeft(Joystick* joystick_p);
bool Joystick_isTappedRight(Joystick* joystick_p);
bool Joystick_isTappedUp(Joystick* joystick_p);
bool Joystick_isTappedDown(Joystick* joystick_p);


// Gets a normalized direction for movement
int Joystick_getXDirection(Joystick* joystick_p);
int Joystick_getYDirection(Joystick* joystick_p);


#endif /* JOYSTICK_H_ */
