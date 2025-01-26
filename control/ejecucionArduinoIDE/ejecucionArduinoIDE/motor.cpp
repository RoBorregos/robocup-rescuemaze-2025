#include "motor.h"
#include <Arduino.h>

void motor::initialize(int in_1,int in_2,int en){
    in1=in_1;
    in2=in_2;
    enable=en;
    pinMode(in1,INPUT);
    pinMode(in2,INPUT);
    pinMode(enable,OUTPUT);
}
motor::motor(){

}
void motor::setSpeed(uint8_t speed){
    analogWrite(enable,speed);
}
void motor::ahead(){
    digitalWrite(in1,1);
    digitalWrite(in2,0);
}
void motor::back(){ 
    digitalWrite(in1,0);
    digitalWrite(in2,1);
}
void motor::stop(){
    digitalWrite(in1,0);
    digitalWrite(in2,0);
}
