#include "Encoder.h"
#include "motors.h"
motors robot;
void Encoder::backRightEncoder() {
    robot.motor[MotorID::kBackRight].updateTics();
}
void Encoder::backLeftEncoder() {
    robot.motor[MotorID::kBackLeft].updateTics();
}
void Encoder::frontRightEncoder() {
    robot.motor[MotorID::kFrontRight].updateTics();
}
void Encoder::frontLeftEncoder() {
    robot.motor[MotorID::kFrontLeft].updateTics();
}
