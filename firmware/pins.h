/**
 * This file contains pin definitions for the TagGang
 * Arduino Pi HAT.
 */
#ifndef PINS_H
#define PINS_H

// ATMEGA328P to Arduino UNO pin definitions
#define PIN_TO_NUMBER(pin) (digitalPinToPCICR(pin) ? (digitalPinToPCMSK(pin) & _BV(digitalPinToPCICRbit(pin))) : (pin))

// Left motor-driver pins
#define PIN_L_NFAULT	PIN_TO_NUMBER(PD4)
#define PIN_L_DIR		PIN_TO_NUMBER(PB0)
#define PIN_L_NENBL		PIN_TO_NUMBER(PD7)
#define PIN_L_STEP		PIN_TO_NUMBER(PD5)
#define PIN_L_NHOME		PIN_TO_NUMBER(PD2)

// Right motor-driver pins
#define PIN_R_NFAULT	PIN_TO_NUMBER(PC0)
#define PIN_R_DIR		PIN_TO_NUMBER(PC2)
#define PIN_R_NENBL		PIN_TO_NUMBER(PC3)
#define PIN_R_STEP		PIN_TO_NUMBER(PD6)
#define PIN_R_NHOME		PIN_TO_NUMBER(PC1)

#define PIN_PEN_SERVO   PIN_TO_NUMBER(PB1)

#endif
