/**
 * This file contains pin definitions for the TagGang
 * Arduino Pi HAT.
 */
#ifndef PINS_H
#define PINS_H

#include <Arduino.h>

// Left motor-driver pins
#define PIN_L_NFAULT	PD4
#define PIN_L_DIR	    PB0
#define PIN_L_NENBL		PD7
#define PIN_L_STEP		PD5
#define PIN_L_NHOME		PD2

// Right motor-driver pins
#define PIN_R_NFAULT	PC0
#define PIN_R_DIR		PC2
#define PIN_R_NENBL		PC3
#define PIN_R_STEP		PD6
#define PIN_R_NHOME		PC1

#define PIN_PEN_SERVO   PB1

#endif
