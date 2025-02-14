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
    // robot.vlx[vlxID::right].getDistance();
    // robot.wifiPrint("angulo",robot.vlx[vlxID::right].distance);
    // m.run_algs();
    // robot.wifiPrint("leftdistance",robot.vlx[vlxID::left].getDistance());
    // delay(200);
    // robot.ahead_ultra();
    m.run_algs();
    // delay(200);
    // robot.PID_encoders(12,1);
    // robot.wifiPrint("dis: ",robot.vlx[vlxID::frontLeft].getDistance());
    // if(robot.vlx[vlxID::frontLeft].isWall()){
    //   robot.right();
    // }else{
    //   robot.ahead();
    // }
    // robot.vlx[vlxID::back].printDistance();

}