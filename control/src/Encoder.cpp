#include "Encoder.h"
#include "Pins.h"
motors robot;
int Encoder::deltaTics[4];
void Encoder::backRightEncoder() {
    robot.motor[MotorID::kBackRight].tics+=1;
    deltaTics[MotorID::kBackRight]+=1;
}
void Encoder::backLeftEncoder() {
    robot.motor[MotorID::kBackLeft].tics+=1;
    deltaTics[MotorID::kBackLeft]+=1;
}
void Encoder::frontRightEncoder() {
    robot.motor[MotorID::kFrontRight].tics+=1;
    deltaTics[MotorID::kFrontRight]+=1;
}
void Encoder::frontLeftEncoder() {
    robot.motor[MotorID::kFrontLeft].tics+=1;
    deltaTics[MotorID::kFrontLeft]+=1;
}

