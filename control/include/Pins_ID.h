#ifndef PINS_ID_H
#define PINS_ID_H
#include <Arduino.h>

namespace Pins{
    constexpr uint8_t encoder[4] = {
        36, // BACK_RIGHT 34   34
        39, // BACK_LEFT 35    35
        34,  // FRONT_RIGHT 36
        35 // FRONT_LEFT 39
    };
    constexpr uint8_t pwmPin[4] = {
        2, // BACK_RIGHT 15   15
        5, // BACK_LEFT 16   5
        26,  // FRONT_RIGHT 26
        32 // FRONT_LEFT 32 
    };

    constexpr uint8_t digitalOne[4] = {
        15, // BACK_RIGHT 4    2
        17, // BACK_LEFT 17   17
        14,  // FRONT_RIGHT 14
        33 // FRONT_LEFT 33   
    };  
    constexpr uint8_t digitalTwo[4] = {
        4, // BACK_RIGHT 2   4
        16, // BACK_LEFT 5  16
        27,  // FRONT_RIGHT 27
        25// FRONT_LEFT 25 
    };
    constexpr uint8_t vlxPins[5] = {//MUX
        // Defined Pins on 20th of March
        3,  // FRONT_LEFT  fine 0  2
        4, // LEFT         fine 1  3    
        0, // BACK         fine 2  0
        2,  // RIGHT       fine 4  5
        5 // FRONT_RIGHT   fine 3  
    };
    constexpr uint8_t tcsPins[1] = {
        6 // 5
    };
    constexpr uint8_t limitSwitchPins[2] = {
        13, // LEFT
        23  // RIGHT
    };
    constexpr uint8_t servoPin = 18;
    constexpr uint8_t checkpointPin = 19;

}
namespace LimitSwitchID {
    constexpr uint8_t kLeft = 0;
    constexpr uint8_t kRight = 1;
}
namespace vlxID{
    constexpr uint8_t right=2;//si
    constexpr uint8_t left=1;//si
    constexpr uint8_t frontRight=5;//no
    constexpr uint8_t frontLeft=3;//si
    constexpr uint8_t front=4;//no
    constexpr uint8_t back=0;//no
}
namespace MotorID{
    constexpr uint8_t kBackRight=2;
    constexpr uint8_t kBackLeft=3;
    constexpr uint8_t kFrontRight=0;//3
    constexpr uint8_t kFrontLeft=1;//////
}
#endif
