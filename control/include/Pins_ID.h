#ifndef PINS_ID_H
#define PINS_ID_H
#include <Arduino.h>

namespace Pins{
    constexpr uint8_t encoder[4] = {
        36, 
        39, 
        34,  
        35 
    };
    constexpr uint8_t pwmPin[4] = {
        2, 
        5, 
        26, 
        32 
    };

    constexpr uint8_t digitalOne[4] = {
        15,
        16, 
        27, 
        25  
    };  
    constexpr uint8_t digitalTwo[4] = {
        4, 
        17, 
        14, 
        33
    };
    constexpr uint8_t vlxPins[5] = {//MUX
        3,  
        4, 
        0, 
        2, 
        5
    };
    constexpr uint8_t tcsPins[1] = {
        6 
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
    constexpr uint8_t right=2;
    constexpr uint8_t left=1;
    constexpr uint8_t frontRight=5;
    constexpr uint8_t frontLeft=3;
    constexpr uint8_t front=4;
    constexpr uint8_t back=0;
    constexpr uint8_t frontCenter=7;

}
namespace MotorID{
    constexpr uint8_t kBackRight=1;
    constexpr uint8_t kBackLeft=3;
    constexpr uint8_t kFrontRight=2;
    constexpr uint8_t kFrontLeft=0;
}
namespace kitID{
    constexpr uint8_t kRight=2;
    constexpr uint8_t kLeft=1;
    constexpr uint8_t kNone=0;
}
namespace rampID{
    constexpr uint8_t kUp=1;
    constexpr uint8_t kDown=2;
    constexpr uint8_t kNone=0;
}
#endif
