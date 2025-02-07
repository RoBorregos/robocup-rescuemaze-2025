#include "motor.h"
#include <Arduino.h>
#include "Pins.h"
#include "Encoder.h"

void motor_::initialize(uint8_t in_1,uint8_t in_2,uint8_t en,uint8_t numMotor){
    in1=in_1;
    in2=in_2;
    enable=en;
    // attachInterrupt(digitalPinToInterrupt(encoderPins[numMotor]),frontLeftEncoder,RISING);
    pinMode(in1,OUTPUT);
    pinMode(in2,OUTPUT);
    pinMode(enable,OUTPUT);
    pinMode(Pins::encoder[numMotor],INPUT);
    initEncoder(numMotor);
}

motor_::motor_(){
    //default constructor
}
void motor_::initEncoder(uint8_t motorId_) {
     switch (motorId_) {
        case (MotorID::kFrontLeft):{
            attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kFrontLeft]), Encoder::frontLeftEncoder, RISING);
            break;
        }
        case (MotorID::kFrontRight): {
            attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kFrontRight]), Encoder::frontRightEncoder, RISING);
            break;
        }
        case (MotorID::kBackLeft): {
            attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kBackLeft]), Encoder::backLeftEncoder, RISING);
            break;
        }
        case (MotorID::kBackRight): {
            attachInterrupt(digitalPinToInterrupt(Pins::encoder[MotorID::kBackRight]), Encoder::backRightEncoder, RISING);
            break;
        }
        default: { 
            break;
        }
    }
}
void motor_::updateTics(){
    tics+=1;
    deltaTics+=1;
    unsigned long calculate_time=100;
    unsigned long current_time=millis()-last_time;
    if(current_time>=calculate_time){
        ticsSpeed=deltaTics;
        deltaTics=0;
        last_time=millis();
    }
}
void motor_::resetTics(){
    tics=0;
    deltaTics=0;
}
int motor_::getTics(){
    return tics;
}
void motor_::setSpeed(uint16_t velocity){
    speed=velocity;
    speed=constrain(speed,0,255);
    analogWrite(enable,speed);
    delay(1);//no quitar(no se porque pero si lo quitas no jala :|..)
}
void motor_::ahead(){
    digitalWrite(in1,1);
    digitalWrite(in2,0);
}
void motor_::back(){ 
    digitalWrite(in1,0);
    digitalWrite(in2,1);
}
void motor_::stop(){
    digitalWrite(in1,0);
    digitalWrite(in2,0);
}
double motor_::getSpeed(){
    return speed;
}
int motor_::getTicsSpeed(){
    return ticsSpeed;
}
