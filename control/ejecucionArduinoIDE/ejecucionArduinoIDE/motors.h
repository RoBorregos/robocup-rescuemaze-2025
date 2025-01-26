#ifndef MOTORS_H
#define MOTORS_H
#include <Arduino.h>
#include "motor.h"
class motors{
private:
    motor motor1;
    motor motor2;
    motor motor3;
    motor motor4;
public:
    motors(int,int,int,int,int,int,int,int,int,int,int,int);
    void setSpeed(uint8_t);
    void ahead();
    void back();
    void left();
    void right();
    void stop();

};
#endif
