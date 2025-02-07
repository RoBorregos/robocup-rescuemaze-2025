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
    uint8_t in1;
    uint8_t in2;
    uint8_t enable;
    uint16_t speed; 
    int ticsSpeed;
    unsigned long last_time;
public:
    volatile int tics;
    volatile int deltaTics;
    void initialize(uint8_t,uint8_t,uint8_t,uint8_t);
    void initEncoder(uint8_t);
    motor_();
    void updateTics();
    void setSpeed(uint16_t);
    void ahead();
    void back();
    void stop();
    double getSpeed();
    void resetTics();
    int getTics();
    int getTicsSpeed();
};
#endif

