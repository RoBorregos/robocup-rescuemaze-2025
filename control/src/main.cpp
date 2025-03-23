#include <Arduino.h>
#include "Encoder.h"
#include "maze.h"
#include "Test.h"
#include <FastLED.h>
#include "Jetson.h"
// #include "Screen.h"
maze m;
Jetson jetson;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  robot.setupMotors();
  // screenBegin();
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kFrontLeft]), Interrups::frontLeftEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kFrontRight]), Interrups::frontRightEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kBackLeft]), Interrups::backLeftEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kBackRight]), Interrups::backRightEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::checkpointPin),Interrups::lackOfProgress, RISING);
  
}

void loop() {
//  robot.tcs_.printRGB();
// screenPrint("hello");
m.run_algs();
// testVlxRight();
// jetson.getDetection();
// delay(6000);
// Serial.println(robot.tcs_.getColor());
//  robot.tcs_.HSV();
// testMotors();
// Serial.println("loop");
// testEncoder(MotorID::kFrontRight);
// testVlx(vlxID::frontLeft);
// Serial.println(robot.vlx[vlxID::frontLeft].getDistance());
// m.run_algs();
// testMotors();
// robot.setahead();
// robot.motor[MotorID::kFrontLeft].setSpeed(100);
// delay(500);
// robot.motor[MotorID::kFrontRight].setSpeed(100);
// delay(500);
// robot.motor[MotorID::kBackLeft].setSpeed(100);
// delay(500);
// robot.motor[MotorID::kBackRight].setSpeed(100);
// delay(500);


}
