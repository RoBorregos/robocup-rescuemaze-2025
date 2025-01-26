#include "motor.h"
#include <Arduino.h>
#include "Pins.h"
#include "Encoder.h"

void motor_::initialize(int in_1,int in_2,int en,int numMotor){
    in1=in_1;
    in2=in_2;
    enable=en;
    // attachInterrupt(digitalPinToInterrupt(encoderPins[numMotor]),frontLeftEncoder,RISING);
    pinMode(in1,OUTPUT);
    pinMode(in2,OUTPUT);
    pinMode(enable,OUTPUT);
    pinMode(Pins::encoder[numMotor],INPUT);
    
}

motor_::motor_(){
    //default constructor
}
void motor_::resetTics(){
    tics=0;
}
int motor_::getTics(){
    return tics;
}
void motor_::setSpeed(int velocity){
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
