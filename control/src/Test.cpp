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
    float distance=robot.vlx[vlxID::frontRight].getDistance();
    Serial.println(distance);
    String print=static_cast<String>(distance);
    robot.screenPrint(print);

}
void testVlxRight(){
    float distance=robot.vlx[vlxID::right].getDistance();
    Serial.println(distance);
    String print=static_cast<String>(distance);
    robot.screenPrint(print);

}
void testVlxLeft(){
    float distance=robot.vlx[vlxID::left].getDistance();
    Serial.println(distance);
    String print=static_cast<String>(distance);
    robot.screenPrint(print);
}
void testVlxBack(){
    float distance=robot.vlx[vlxID::back].getDistance();
    Serial.println(distance);
    String print=static_cast<String>(distance);
    robot.screenPrint(print);
}
void testVlxFront(){
    float distance=robot.vlx[vlxID::front].getDistance();
    Serial.println(distance);
    String print=static_cast<String>(distance);
    robot.screenPrint(print);
}
void testVlxFrontDistance(){
    robot.vlx[vlxID::frontLeft].getDistance();
    robot.vlx[vlxID::frontRight].getDistance();
    float frontDistance=(robot.vlx[vlxID::frontLeft].distance>robot.vlx[vlxID::frontRight].distance) ? robot.vlx[vlxID::frontLeft].distance:robot.vlx[vlxID::frontRight].distance;
    String print = static_cast<String>(frontDistance);
    robot.screenPrint(print);
    Serial.println(frontDistance);

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
    Serial.println(robot.tcs_.red_);
    Serial.println(robot.tcs_.green_);
    Serial.println(robot.tcs_.blue_);
    delay(1000);
}
void testPIDWheel(){
    robot.setahead();
    robot.PID_Wheel(20,MotorID::kFrontLeft);
}

void testLimits(){
    Serial.println(robot.limitSwitch_[LimitSwitchID::kLeft].getState());
    Serial.println(robot.limitSwitch_[LimitSwitchID::kRight].getState());
}
void testButton(){
    Serial.println(robot.buttonPressed);
}
void testBnoY(){
    String print=static_cast<String>(robot.bno.getOrientationY());
    robot.screenPrint(print);
    Serial.println(print);
}
void calibrateColors(){
    robot.calibrateColors();
}
void pidTest(){
    robot.setahead();
    robot.pidEncoders(20,true);
}