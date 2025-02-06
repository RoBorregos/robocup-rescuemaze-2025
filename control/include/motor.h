#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

namespace MotorID{
    constexpr uint8_t kBackRight=0;
    constexpr uint8_t kBackLeft=1;
    constexpr uint8_t kFrontRight=2;
    constexpr uint8_t kFrontLeft=3;
}
class motor_{
private:
    int in1;
    int in2;
    uint8_t enable;
    uint32_t speed; 
    int ticsSpeed;
    unsigned long last_time;
public:
    volatile int tics;
    volatile int deltaTics;
    void initialize(int,int,int,int);
    void initEncoder(int);
    motor_();
    void updateTics();
    void setSpeed(int);
    void ahead();
    void back();
    void stop();
    double getSpeed();
    void resetTics();
    int getTics();
    int getTicsSpeed();
};
#endif

