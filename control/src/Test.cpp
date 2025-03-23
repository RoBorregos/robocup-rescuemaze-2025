#include "Test.h"
void testEncoder(uint8_t id){
    robot.motor[id].ahead();
    robot.motor[id].setSpeed(100);
    Serial.println(robot.motor[id].tics);
}
void testVlx(uint8_t id){
    Serial.println(robot.vlx[id].getDistance());
}
void testVlxFrontLeft(){
    Serial.println(robot.vlx[vlxID::frontLeft].getDistance());
}
void testVlxFrontRigth(){
    Serial.println(robot.vlx[vlxID::frontRight].getDistance());

}
void testVlxRight(){
    Serial.println(robot.vlx[vlxID::right].getDistance());

}
void testVlxLeft(){
    Serial.println(robot.vlx[vlxID::left].getDistance());
}
void testVlxBack(){
    Serial.println(robot.vlx[vlxID::back].getDistance());
}
void testVlxFront(){
    Serial.println(robot.vlx[vlxID::front].getDistance());
}
void testMotors(){
    robot.setahead();
    Serial.println("FrontLeft");
    robot.motor[MotorID::kFrontLeft].setSpeed(100);
    delay(500);
    Serial.println("FrontRight");
    robot.motor[MotorID::kFrontRight].setSpeed(100);
    delay(500);
    Serial.println("BackLeft");
    robot.motor[MotorID::kBackLeft].setSpeed(100);
    delay(500);
    Serial.println("BackRight");
    robot.motor[MotorID::kBackRight].setSpeed(100);
    delay(500);
    robot.setSpeed(0);
}