#include "Test.h"
void testEncoder(uint8_t id){
    robot.motor[id].ahead();
    robot.motor[id].setSpeed(100);
    Serial.println(robot.motor[id].tics);
}
void testVlx(uint8_t id){
    // Serial.print("Tics motor ");
    // Serial.println(id);
    Serial.println(robot.vlx[id].getDistance());
}