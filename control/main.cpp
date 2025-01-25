#include <Arduino.h>

#include "Encoder.h"
// #include "motors.h"

// #include "Encoder.h"
// #include "VLX.h"
// #include <Adafruit_VL53L0X.h>
// #include "BNO.H"
// BNO bno;
// motors myMotors;
// VLX vlx;
// ultrasonico ultra;
// ultrasonico myUltra;
// float targetAngle=0;
double calculate_time=20;
double last_time;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // pinMode(15,OUTPUT);
  // delay(1000);
  // bno.setupBNO();
  // pinMode(2,OUTPUT);
  // vlx.begin();
  robot.setupMotors();
}
void loop() {
  if(robot.vlx[vlxID::frontLeft].isWall()){
    robot.right();
  }else{
    robot.ahead_ultra();
  }
  // delay(100);
  // Serial.println(robot.vlx[MotorID::kFrontLeft].getDistance());
  // robot.right();
  // delay(700);

 
  // robot.printAngle();
  // robot.vlx[0].printDistance();
  // robot.setback();
  // robot.ahead();
  // delay(700);
  // robot.ahead_ultra();
  // delay(700);
  // robot.ahead_ultra();
  // delay(700);
  // robot.ahead_ultra();
  // delay(20000);
  // robot.right();
  // delay(700);

  
}
