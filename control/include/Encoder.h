
#ifndef Encoder_h
#define Encoder_h

#include <Arduino.h>
#include "motors.h"
#include "Pins_ID.h"
extern motors robot;
namespace Interrups {
    void backRightEncoder();
    void backLeftEncoder();
    void frontRightEncoder();
    void frontLeftEncoder();
    void lackOfProgress();
    extern int deltaTics[4];
};

#endif