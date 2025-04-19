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
        4, // BACK_RIGHT 4    2
        16, // BACK_LEFT 17   17
        27,  // FRONT_RIGHT 14
        33 // FRONT_LEFT 33   
    };  
    constexpr uint8_t digitalTwo[4] = {
        15, // BACK_RIGHT 2   4
        17, // BACK_LEFT 5  16
        14,  // FRONT_RIGHT 27
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
    constexpr uint8_t LedsPin = 12;


}
namespace LimitSwitchID {
    constexpr uint8_t kLeft = 0;
    constexpr uint8_t kRight = 1;
}
namespace vlxID{
    constexpr uint8_t right=5;//si
    constexpr uint8_t left=3;//si
    constexpr uint8_t frontRight=0;//no
    constexpr uint8_t frontLeft=2;//si
    constexpr uint8_t front=1;//no
    constexpr uint8_t back=4;//si
    constexpr uint8_t frontCenter=7;//no

}
namespace MotorID{
    constexpr uint8_t kBackRight=1;
    constexpr uint8_t kBackLeft=3;
    constexpr uint8_t kFrontRight=2;//3
    constexpr uint8_t kFrontLeft=0;//////
}
namespace kitID{
    constexpr uint8_t kRight=1;
    constexpr uint8_t kLeft=2;
    constexpr uint8_t kNone=0;//3
}
namespace rampID{
    constexpr uint8_t kUp=1;
    constexpr uint8_t kDown=2;
    constexpr uint8_t kNone=0;//3
}
#endif
