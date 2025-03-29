#include "Encoder.h"
#include "motors.h"
motors robot;
// Jetson jetson;
void Interrups::backRightEncoder() {
    robot.motor[MotorID::kBackRight].updateTics();
}
void Interrups::backLeftEncoder() {
    robot.motor[MotorID::kBackLeft].updateTics();
}
void Interrups::frontRightEncoder() {
    robot.motor[MotorID::kFrontRight].updateTics();
}
void Interrups::frontLeftEncoder() {
    robot.motor[MotorID::kFrontLeft].updateTics();
}
void Interrups::lackOfProgress(){
    robot.buttonPressed=true;
}

