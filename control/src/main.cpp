#include <Arduino.h>
#include "Encoder.h"
#include "maze.h"
#include <FastLED.h>
#include "Jetson.h"
maze m;
Jetson jetson;
// TaskHandle_t Task0;
// void loop0(void *parameter);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  robot.setupMotors();
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kFrontLeft]), Encoder::frontLeftEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kFrontRight]), Encoder::frontRightEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kBackLeft]), Encoder::backLeftEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kBackRight]), Encoder::backRightEncoder, RISING);
  // xTaskCreatePinnedToCore(loop0,"Task_0",4096,NULL,1,&Task0,0);
  
}

void loop() {
//  robot.tcs_.printRGB();
// m.run_algs();
jetson.getDetection();
// delay(6000);
// Serial.println(robot.tcs_.getColor());
//  robot.tcs_.HSV();
}

// void loop0(void *parameter){
//   Serial.println("hola");
//   while(true){
//     unsigned long lastTime=millis();
//     robot.vlx[vlxID::frontLeft].getDistance();
//     robot.vlx[vlxID::right].getDistance();
//     robot.vlx[vlxID::left].getDistance();
//     robot.vlx[vlxID::back].getDistance();
//     unsigned long time=millis()-lastTime;
//     // Serial.println(time);
//   }
//   vTaskDelay(10 / portTICK_PERIOD_MS);
// }
