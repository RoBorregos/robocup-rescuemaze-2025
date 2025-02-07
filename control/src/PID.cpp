#include "PID.H"
#include <Arduino.h>
#include "Encoder.h"
PID::PID(float kp_,float ki_,float kd_,float calculate_time_){
    kp=kp_;
    ki=ki_;
    kd=kd_;
    calculate_time=calculate_time_;
}
PID::PID(){
    kp=6.0;
    ki=0.1;
    kd=0.8;
}
void PID::changeConstants(float kp_,float ki_,float kd_,float calculate_time_){
    kp=kp_;
    ki=ki_;
    kd=kd_;
    calculate_time=calculate_time_;
}
double PID::calculate_PID(float setpoint, float input){
    float current_time=millis();
    float delta_time=current_time-last_time;
    if(delta_time>=calculate_time){
        float error=setpoint-input;
        float total_error=error+last_error;

        float proportional=kp*error;
        float integral=ki*total_error;
        float derivative=kd*(error-last_error)/(delta_time);
        float output=proportional+integral+derivative;
        last_error=error;
        last_time=current_time;
        return output;
    }
    return 0;
}

