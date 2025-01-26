#ifndef PINS_H
#define PINS_H
#include <Arduino.h>

namespace Pins{
    constexpr uint8_t encoder[4] = {
        34, // BACK_RIGHT 34   34
        35, // BACK_LEFT 35    35
        36,  // FRONT_RIGHT 36
        39 // FRONT_LEFT 39
    };
    constexpr uint8_t pwmPin[4] = {
        15, // BACK_RIGHT 15   15
        5, // BACK_LEFT 16   5
        26,  // FRONT_RIGHT 26
        32 // FRONT_LEFT 32 
    };

    constexpr uint8_t digitalOne[4] = {
        4, // BACK_RIGHT 4    2
        16, // BACK_LEFT 17   17
        14,  // FRONT_RIGHT 14
        33 // FRONT_LEFT 33   
    };  
    constexpr uint8_t digitalTwo[4] = {
        2, // BACK_RIGHT 2   4
        17, // BACK_LEFT 5  16
        27,  // FRONT_RIGHT 27
        25 // FRONT_LEFT 25 
    };
    constexpr uint8_t vlxPins[5] = {//MUX
        // Defined Pins on 20th of March
        3,  // FRONT_LEFT  fine 0  2
        4, // LEFT         fine 1  3    
        0, // BACK         fine 2  0
        2,  // RIGHT       fine 4  5
        5 // FRONT_RIGHT   fine 3  
    };
}

#endif

    ////////candidate-robot////////
    // constexpr uint8_t pwmPin[4] = {
    //     7,  // BACK_RIGHT (ENA_MT1)
    //     5,  // BACK_LEFT (ENA_MT2)
    //     6,  // FRONT_RIGHT (ENA_MT3)
    //     10  // FRONT_LEFT (ENA_MT4)
    // };

    // constexpr uint8_t digitalOne[4] = {
    //     33, // BACK_RIGHT (IN1_MT1)
    //     31, // BACK_LEFT (IN1_MT2)
    //     29, // FRONT_RIGHT (IN1_MT3)
    //     27  // FRONT_LEFT (IN1_MT4)
    // };

    // constexpr uint8_t digitalTwo[4] = {
    //     32, // BACK_RIGHT (IN2_MT1)
    //     30, // BACK_LEFT (IN2_MT2)
    //     28, // FRONT_RIGHT (IN2_MT3)
    //     26  // FRONT_LEFT (IN2_MT4)
    // };
//////////////////////////////////
//    const int ultrasonico[2]={
//     2,//trig
//     3,//echo
//     }; 
    // constexpr uint8_t encoder[4] = {
    //     0, // BACK_RIGHT 34   34
    //     0, // BACK_LEFT 35    35
    //     18,  // FRONT_RIGHT 36
    //     19 // FRONT_LEFT 39
    // };