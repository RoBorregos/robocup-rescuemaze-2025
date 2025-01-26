#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

class motor{
private:
   int in1;
   int in2;
   int enable;
public:
    void initialize(int,int,int);
    motor();
    void setSpeed(uint8_t);
    void ahead();
    void back();
    void stop();

};
#endif

