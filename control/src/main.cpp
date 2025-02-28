#include <Arduino.h>
#include "Encoder.h"
#include "maze.h"
maze m;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  robot.setupMotors();
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kFrontLeft]), Encoder::frontLeftEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kFrontRight]), Encoder::frontRightEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kBackLeft]), Encoder::backLeftEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kBackRight]), Encoder::backRightEncoder, RISING);
  
}
void loop() {
  m.run_algs();

}

