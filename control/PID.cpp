#include "PID.H"
#include <Arduino.h>
#include "Encoder.h"
PID::PID(double kp_,double ki_,double kd_,double calculate_time_){
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
void PID::changeConstants(double kp_,double ki_,double kd_,double calculate_time_){
    kp=kp_;
    ki=ki_;
    kd=kd_;
    calculate_time=calculate_time_;
}
double PID::calculate_PID(double setpoint, double input){
    double current_time=millis();
    double delta_time=current_time-last_time;
    if(delta_time>=calculate_time){
        double error=setpoint-input;
        double total_error=error+last_error;

        double proportional=kp*error;
        double integral=ki*total_error;
        double derivative=kd*(error-last_error)/(delta_time);
        double output=proportional+integral+derivative;
        last_error=error;
        last_time=current_time;
        return output;
    }
    return 0;
}

