#include <Arduino.h>
#include "Encoder.h"
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  robot.setupMotors();
}
void loop() {
  if(robot.vlx[vlxID::frontLeft].isWall()){
    robot.right();
  }else{
    robot.ahead_ultra();
  }
  
}
