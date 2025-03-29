#include <Arduino.h>
#include "Encoder.h"
#include "maze.h"
#include "Test.h"
#include <FastLED.h>
#include "Jetson.h"
maze m;

void setup() {
  // put your setup code here, to run once:
  pinMode(2,OUTPUT);
  digitalWrite(2,1);
  Serial.begin(115200);
  robot.setupMotors();
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kFrontLeft]), Interrups::frontLeftEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kFrontRight]), Interrups::frontRightEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kBackLeft]), Interrups::backLeftEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kBackRight]), Interrups::backRightEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::checkpointPin),Interrups::lackOfProgress, RISING);
  
}

void loop() {
  // m.getDetectionJetson();
  digitalWrite(2,1);

  m.run_algs();
  // delay(3000);
  // float aglueee=robot.getAngleOrientation();
  // robot.rotate(aglueee);
  // robot.checkpointElection(); 
  // robot.buttonPressed=false;
  // Serial.println(robot.limitSwitch_[LimitSwitchID::kLeft].getState());
  // Serial.println(robot.limitSwitch_[LimitSwitchID::kRight].getState());
  // jeetson.getDetection();
  // jetson.getDetection();
  // testTCS();
  // testMotors();
  // testEncoders();
  // testVlxFrontLeft();
  // testVlxFrontRigth();
  // testVlxRight();
  // testVlxLeft();
  // testVlxFront();
  // testVlxBack();
}