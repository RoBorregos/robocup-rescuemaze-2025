#ifndef PID_H
#define PID_H

class PID{
private:
    double kp;
    double ki;
    double kd;
    double last_error;
    double last_time;

public:
    PID(double,double,double,double);
    PID();
    double calculate_PID(double,double);
    void changeConstants(double,double,double,double);
    double calculate_time=20;

};
#endif