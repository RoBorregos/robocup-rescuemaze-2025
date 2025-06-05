#ifndef PID_H
#define PID_H

class PID{
private:
    float kp;
    float ki;
    float kd;
    float last_error;
    float last_time;

public:
    PID(float,float,float,float);
    PID();
    double calculate_PID(float,float);
    void changeConstants(float,float,float,float);
    float calculate_time=20;

};
#endif