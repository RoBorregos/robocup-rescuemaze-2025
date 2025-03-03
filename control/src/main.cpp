#include <Arduino.h>
#include "Encoder.h"
#include "maze.h"
maze m;
TaskHandle_t Task0;
void loop0(void *parameter);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // robot.setupMotors();
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kFrontLeft]), Encoder::frontLeftEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kFrontRight]), Encoder::frontRightEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kBackLeft]), Encoder::backLeftEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kBackRight]), Encoder::backRightEncoder, RISING);
  // xTaskCreatePinnedToCore(loop0,"Task_0",4096,NULL,1,&Task0,0);
  
}
void loop() {
  // Serial.println(robot.vlx[vlxID::right].distance);
  // delay(10);
  // m.run_algs();
  Serial.println("P");
  if(Serial.available()){
    String message=Serial.readString();
    Serial.println(message);
  }
  delay(2000);
  // Serial.println(robot.getAngleOrientation());
  // robot.ramp();
  // robot.tcs_.printColor();

}
void limpiarBuffer() {
  while (Serial.available() > 0) {
      Serial.read();  // Leer y descartar el byte disponible
  }
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
