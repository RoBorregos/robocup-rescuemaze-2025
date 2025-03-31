#include "Test.h"
void testEncoder(uint8_t id){
    robot.motor[id].ahead();
    robot.motor[id].setSpeed(100);
    Serial.println(robot.motor[id].tics);
}
void testEncoders(){
    String print=static_cast<String>(robot.getAvergeTics());
    Serial.println(print);
    robot.screenPrint(print);
}
void testVlx(uint8_t id){
    Serial.println(robot.vlx[id].getDistance());
}
void testVlxFrontLeft(){
    float distance=robot.vlx[vlxID::frontLeft].getDistance();
    Serial.println(distance);
    String print=static_cast<String>(distance);
    robot.screenPrint(print);
}
void testVlxFrontRigth(){
    Serial.println(robot.vlx[vlxID::frontRight].getDistance());

}
void testVlxRight(){
    Serial.println(robot.vlx[vlxID::right].getDistance());

}
void testVlxLeft(){
    float distance=robot.vlx[vlxID::left].getDistance();
    Serial.println(distance);
    String print=static_cast<String>(distance);
    robot.screenPrint(print);
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
    robot.motor[MotorID::kFrontLeft].setSpeed(200);
    delay(500);
    Serial.println("FrontRight");
    robot.motor[MotorID::kFrontRight].setSpeed(200);
    delay(500);
    Serial.println("BackLeft");
    robot.motor[MotorID::kBackLeft].setSpeed(200);
    delay(500);
    Serial.println("BackRight");
    robot.motor[MotorID::kBackRight].setSpeed(200);
    delay(500);
    robot.setSpeed(0);
}
void testTCS(){
    String print=static_cast<String>(robot.tcs_.getColor());
    robot.screenPrint(print);
    Serial.println(print);
    delay(1000);
}
void testPIDWheel(){
    robot.setahead();
    robot.PID_Wheel(20,MotorID::kFrontLeft);
}

void testButton(){
    Serial.println(robot.buttonPressed);
}