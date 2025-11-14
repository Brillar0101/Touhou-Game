/*
 *  Implementation of Joystick HAL
 *  Author : Barakaeli Lawuo
 *  Date : November 13th 2025
 *
 */

#include <HAL/Joystick.h>

//helper functions
static void initADC();
static void startADC();
static void updateTapStates(Joystick* joystick_p);

// Initialize ADC to help with the Joysyick
static void initADC() {

    // Enables ADC Module
    ADC14_enableModule();

    //Initializes ADC with a SMCLK clock source
    ADC14_initModule(ADC_CLOCKSOURCE_SMCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);

    // Configures GPIO pins for the analog input
    // P6.0 is Joystick X-axis
    // P4.4 is Joystick Y-axis

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0,GPIO_TERTIARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4,GPIO_TERTIARY_MODULE_FUNCTION);

    // Configures ADC memory for X-axis
    ADC14_configureConversionMemory(ADC_MEM0,
                                     ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                     ADC_INPUT_A15,
                                     ADC_NONDIFFERENTIAL_INPUTS);
    ADC14_configureConversionMemory(ADC_MEM1,
                                     ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                     ADC_INPUT_A9,
                                     ADC_NONDIFFERENTIAL_INPUTS);

    // Configures multi-sample mode from MEM0 to MEM1
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);

    // Sets a sample timer to automatic
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
}


//  Begin ADC Conversion
static void startADC(){
    ADC14_enableConversion();
}

// Updates the tap detection FSM which wehere  called in the FSM
static void updateTapStates(Joystick* joystick_p){
    // Left tap on FSM
    switch(joystick_p->tapLeftState){
        case NOT_TAPPED:
            if(joystick_p-> x < JOYSTICK_LEFT_THRESHOLD){
                joystick_p->tapLeftState = TAPPED;
                joystick_p->isTappedLeft = true;
            } else{
                joystick_p->isTappedLeft = false;
            }
            break;

        case TAPPED:
            if (joystick_p-> x > JOYSTICK_LEFT_THRESHOLD){
                joystick_p-> tapLeftState = NOT_TAPPED;
            }
            joystick_p->isTappedLeft = false;
            break;
    }

    //  Right tap on FSM
    switch(joystick_p->tapRightState){
        case NOT_TAPPED:
            if(joystick_p-> x > JOYSTICK_RIGHT_THRESHOLD){
                joystick_p->tapRightState = TAPPED;
                joystick_p->isTappedRight = true;
            } else{
                joystick_p->isTappedRight = false;
            }
            break;

        case TAPPED:
            if (joystick_p-> x < JOYSTICK_RIGHT_THRESHOLD){
                joystick_p-> tapRightState = NOT_TAPPED;
            }
            joystick_p->isTappedRight = false;
            break;
    }

    //  Up tap on FSM
    switch(joystick_p->tapUpState){
        case NOT_TAPPED:
            if(joystick_p-> y < JOYSTICK_UP_THRESHOLD){
                joystick_p->tapUpState = TAPPED;
                joystick_p->isTappedUp = true;
            } else{
                joystick_p->isTappedUp = false;
            }
            break;

        case TAPPED:
            if (joystick_p-> y > JOYSTICK_UP_THRESHOLD){
                joystick_p-> tapUpState = NOT_TAPPED;
            }
            joystick_p->isTappedUp = false;
            break;
    }

    //  Down tap on FSM
    switch(joystick_p->tapDownState){
        case NOT_TAPPED:
            if(joystick_p-> y > JOYSTICK_DOWN_THRESHOLD){
                joystick_p->tapDownState = TAPPED;
                joystick_p->isTappedDown = true;
            } else{
                joystick_p->isTappedDown = false;
            }
            break;

        case TAPPED:
            if (joystick_p-> y < JOYSTICK_DOWN_THRESHOLD){
                joystick_p-> tapDownState = NOT_TAPPED;
            }
            joystick_p->isTappedDown = false;
            break;
    }
}

// Constructor to initialize the joystick
Joystick Joystick_construct(){
    Joystick joystick;

    // Initialize the readings to center
    joystick.x = JOYSTICK_CENTER;
    joystick.y = JOYSTICK_CENTER;

    // Initialize the tap states
    joystick.tapLeftState = NOT_TAPPED;
    joystick.tapRightState = NOT_TAPPED;
    joystick.tapUpState = NOT_TAPPED;
    joystick.tapDownState = NOT_TAPPED;

    // Initialize tap outputs
    joystick.isTappedLeft = false;
    joystick.isTappedRight = false;
    joystick.isTappedUp = false;
    joystick.isTappedDown = false;

    // Initialize ADC hardware
    initADC();
    startADC();

    return joystick;

}
// Reads ADC and updates the state
void Joystick_refresh(Joystick* joystick_p){
    // Triggers ADC conversion
    ADC14_toggleConversionTrigger();

    // Reads x-axis
    joystick_p->x = ADC14_getResult(ADC_MEM0);

    // Reads y-axis
    joystick_p->y = ADC14_getResult(ADC_MEM1);

    // Updates tap detection on the FSM
    updateTapStates(joystick_p);
}

// Checks if pressed left
bool Joystick_isPressedLeft(Joystick* joystick_p){
    return (joystick_p->x < JOYSTICK_LEFT_THRESHOLD);
}

// Checks if pressed right
bool Joystick_isPressedRight(Joystick* joystick_p){
    return (joystick_p->x > JOYSTICK_RIGHT_THRESHOLD);
}

// Checks if pressed up
bool Joystick_isPressedUp(Joystick* joystick_p){
    return (joystick_p->y < JOYSTICK_UP_THRESHOLD);
}

// Checks if pressed down
bool Joystick_isPressedDown(Joystick* joystick_p){
    return (joystick_p->y > JOYSTICK_DOWN_THRESHOLD);
}

// Checks if tapped left
bool Joystick_isTappedLeft(Joystick* joystick_p){
    return joystick_p->isTappedLeft;
}

// Checks if pressed right
bool Joystick_isTappedRight(Joystick* joystick_p){
    return joystick_p->isTappedRight;
}

// Checks if pressed up
bool Joystick_isTappedUp(Joystick* joystick_p){
    return joystick_p->isTappedUp;
}

// Checks if pressed down
bool Joystick_isTappedDown(Joystick* joystick_p){
    return joystick_p->isTappedDown;
}

// Gets the X-direction
int Joystick_getXDirection(Joystick* joystick_p){
    if(joystick_p->x < JOYSTICK_CENTER - JOYSTICK_DEADZONE){
        return -1;  // Left
    } else if(joystick_p->x > JOYSTICK_CENTER + JOYSTICK_DEADZONE){
        return 1;   // Right
    } else {
        return 0;   // Center
    }
}

// Gets the Y-direction
int Joystick_getYDirection(Joystick* joystick_p){
    if(joystick_p->y < JOYSTICK_CENTER - JOYSTICK_DEADZONE){
        return 1;   // Up
    } else if(joystick_p->y > JOYSTICK_CENTER + JOYSTICK_DEADZONE){
        return -1;  // Down
    } else {
        return 0;   // Center
    }
}
