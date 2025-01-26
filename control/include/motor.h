#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

namespace MotorID{
    constexpr uint8_t kBackRight=0;
    constexpr uint8_t kBackLeft=1;
    constexpr uint8_t kFrontLeft=3;
    constexpr uint8_t kFrontRight=2;
}
class motor_{
private:
    int in1;
    int in2;
    uint8_t enable;
    uint32_t speed; 
public:
    volatile int tics;
    void initialize(int,int,int,int);
    motor_();
    void setSpeed(int);
    void ahead();
    void back();
    void stop();
    double getSpeed();
    void resetTics();
    int getTics();
};
#endif

