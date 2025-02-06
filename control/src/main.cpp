#include <Arduino.h>
#include "Encoder.h"
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  robot.setupMotors();
}
void loop() {

    // Serial.println(robot.motor[0].tics);
    // robot.vlx[vlxID::left].printDistance();
    robot.vlx[vlxID::right].printDistance();
    // robot.vlx[vlxID::back].printDistance();

}