#include <Arduino.h>
// #include "Encoder.h"
#include "maze.h"
maze m;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  robot.setupMotors();
}
void loop() {

    // Serial.println(robot.motor[0].tics);
    // robot.vlx[vlxID::left].printDistance();
    // robot.vlx[vlxID::right].printDistance();
    // robot.rotate(90);
    m.run_algs();
    // robot.vlx[vlxID::back].printDistance();

}