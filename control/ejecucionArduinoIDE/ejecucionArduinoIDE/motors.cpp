#include "motors.h"

motors::motors(int m1P1,int m1P2,int m2P1,int m2P2,int m3P1,int m3P2,int m4P1,int m4P2,int en1,int en2,int en3,int en4){
    motor1.initialize(m1P1,m1P2,en1);
    motor2.initialize(m2P1,m2P2,en2);
    motor3.initialize(m3P1,m3P2,en3);
    motor4.initialize(m4P1,m4P2,en4);
}
void motors::setSpeed(uint8_t speed){
    motor1.setSpeed(speed);
    motor2.setSpeed(speed);
    motor3.setSpeed(speed);
    motor4.setSpeed(speed);
}
void motors::ahead(){
    motor1.ahead();
    motor2.ahead();
    motor3.ahead();
    motor4.ahead();
}
void motors::back(){
    motor1.back();
    motor2.back();
    motor3.back();
    motor4.back();
}
void motors::right(){
    motor1.ahead();
    motor2.back();
    motor3.ahead();
    motor4.back();

}
void motors::left(){
    motor1.back();
    motor2.ahead();
    motor3.back();
    motor4.ahead();
}
void motors::stop(){
    motor1.stop();
    motor2.stop();
    motor3.stop();
    motor4.stop();
}
