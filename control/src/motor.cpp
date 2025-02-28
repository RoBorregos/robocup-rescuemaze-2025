#include "Motor.h"

void Motor::initialize(uint8_t in_1,uint8_t in_2,uint8_t en,uint8_t numMotor){
    in1=in_1;
    in2=in_2;
    enable=en;
    // attachInterrupt(digitalPinToInterrupt(encoderPins[numMotor]),frontLeftEncoder,RISING);
    pinMode(in1,OUTPUT);
    pinMode(in2,OUTPUT);
    pinMode(enable,OUTPUT);
    pinMode(Pins::encoder[numMotor],INPUT);
}

Motor::Motor(){
    //default constructor
}
void Motor::updateTics(){
    tics+=1;
    deltaTics+=1;
    unsigned long calculate_time=40;
    unsigned long current_time=millis()-last_time;
    if(current_time>=calculate_time){
        ticsSpeed=deltaTics;
        deltaTics=0;
        last_time=millis();
    }
}
void Motor::resetTics(){
    tics=0;
    deltaTics=0;
}
int Motor::getTics(){
    return tics;
}
void Motor::setSpeed(uint16_t velocity){
    speed=velocity;
    speed=constrain(speed,0,255);
    analogWrite(enable,speed);
    delay(1);//no quitar(no se porque pero si lo quitas no jala :|..)
}
void Motor::ahead(){
    digitalWrite(in1,1);
    digitalWrite(in2,0);
}
void Motor::back(){ 
    digitalWrite(in1,0);
    digitalWrite(in2,1);
}
void Motor::stop(){
    digitalWrite(in1,0);
    digitalWrite(in2,0);
}
double Motor::getSpeed(){
    return speed;
}
int Motor::getTicsSpeed(){
    return ticsSpeed;
}
