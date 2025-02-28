
#ifndef Encoder_h
#define Encoder_h

#include <Arduino.h>
#include "motors.h"
#include "Pins_ID.h"
extern motors robot;
namespace Encoder {
    void backRightEncoder();
    void backLeftEncoder();
    void frontRightEncoder();
    void frontLeftEncoder();
    extern int deltaTics[4];
};

#endif