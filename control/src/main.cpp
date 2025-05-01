#include <Arduino.h>
#include "Encoder.h"
#include "maze.h"
#include "Test.h"
#include <FastLED.h>
#include "Jetson.h"
maze m;
Jetson jeetson;

void setup() {
  // put your setup code here, to run once:
  pinMode(2,OUTPUT);
  digitalWrite(2,1);
  Serial.begin(115200);
  robot.setupMotors();
  pinMode(Pins::checkpointPin,INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kFrontLeft]), Interrups::frontLeftEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kFrontRight]), Interrups::frontRightEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kBackLeft]), Interrups::backLeftEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kBackRight]), Interrups::backRightEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::checkpointPin),Interrups::lackOfProgress, RISING);
}

void loop() {
  // robot.moveDistance(kTileLength/2,true);
  // // robot.leds.sequency();
  // robot.kitLeft(1);
  // robot.kitRight(1);
  // robot.servo.write(90);
  m.run_algs();
  // testButton();
  // robot.ahead();
  // testMotors();
  // robot.kitLeft(2);
  // testPIDWheel();

  // robot.ahead();
  // robot.ahead();
  // delay(100000000);
  // pidTest();
  // calibrateColors();
  // robot.checkpointElection(); 
  // robot.buttonPressed=false;
  // Serial.println(robot.limitSwitch_[LimitSwitchID::kLeft].getState());
  // Serial.println(robot.limitSwitch_[LimitSwitchID::kRight].getState());
  // jeetson.getDetection();
  // testTCS();
  // testLimits();
  // testBnoY();

  // testMotors();
  // testEncoders();
  // testVlxFrontDistance();
  // testVlxFrontLeft();
  // testVlxFrontRigth();
  // testVlxRight();
  // testVlxLeft();
  // testVlxFront();
  // testVlxBack();
}
