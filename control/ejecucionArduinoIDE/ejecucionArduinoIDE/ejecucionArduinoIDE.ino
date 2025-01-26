#include <Arduino.h>
#include "motors.h"
#include "MPU6050.h"
const int ENA_MT1 = 10;//atras derecha
const int ENA_MT2 = 5;//atras izq
const int ENA_MT3 = 6;//adelante deracha
const int ENA_MT4 = 7;//adelante izq
const int IN1_MT1 = 26;//atras der
const int IN2_MT1 = 27;
const int IN1_MT2 = 28;//atras izq
const int IN2_MT2 = 29;
const int IN1_MT3 = 30;//adelante deracha
const int IN2_MT3 = 31;
const int IN1_MT4 = 32;//adelante izq
const int IN2_MT4 = 33;

// put function declarations here:

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
}

void loop() {
  MPU6050 mpu;
  mpu.inicializarMPU6050();
  while(true){
    mpu.loop_mpu();
    Serial.print(mpu.z_rotation);
  }
  Serial.println("hola");
 
  // put your main code here, to run repeatedly:
  // motors myMotors(IN1_MT1,IN2_MT1,IN1_MT2,IN2_MT2,IN1_MT3,IN2_MT3,IN1_MT4,IN2_MT4,ENA_MT1,ENA_MT2,ENA_MT3,ENA_MT4);
  // uint8_t vel=150;
  // myMotors.setSpeed(vel);
  // while(true){ 
  //   myMotors.ahead();
  // }
  
}



