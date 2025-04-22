#include <Arduino.h>
#include "Encoder.h"
#include "maze.h"
#include "Test.h"
#include <FastLED.h>
#include "Jetson.h"
maze m;
Jetson jeetson;
TaskHandle_t Task0;
// void loop0(void *parameter){
//   // Serial.println("hola");
//   while(true){
//     // if(robot.inMotion==true){
//       robot.checkTileColor();
//     // }
//     // Serial.println(time);
//   }
//   // vTaskDelay(10 / portTICK_PERIOD_MS);
// }
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
  // xTaskCreatePinnedToCore(loop0,"Task_0",4096,NULL,2,&Task0,1);
}

void loop() {

  // // robot.leds.sequency();
  m.run_algs();

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
