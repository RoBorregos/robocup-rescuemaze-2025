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