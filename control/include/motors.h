#ifndef MOTORS_H
#define MOTORS_H
#include <Arduino.h>
#include "motor.h"
#include "PID.H"
#include "VLX.h"
#include "BNO.H"
#include <SPI.h>
class motors{
private:
    int edgeTileDistance=8.5;
    float targetAngle=0;
    PID myPID[4];
    //vlx
    static constexpr int kNumVlx=5;
    double maxVlxDistance=120;
    //wheels
    const double wheelDiameter=8;
    const double distancePerRev=wheelDiameter*3.1416;
    const double kTicsPerRev=496.0;
    const double kTicsPerTile=30*kTicsPerRev/distancePerRev;
    //speed constants
    const uint8_t kMinSpeedRotate=70;
    const uint8_t kMaxSpeedRotate=160;
    const uint8_t kMinSpeedFormard=70;
    const uint8_t kMaxSpeedFormard=180;
    float minDisToLateralWall=3;
    float maxChangeAngle=10;
public:
    BNO bno;
    VLX vlx[kNumVlx];
    motor_ motor[4];//0-BACK_RIGHT//1-BACK_LEFT//2-FRONT_RIGHT//3-FRONT_LEFT
    motors();
    void setupMotors();
    void PID_speed(double, double, int);
    void PID_encoders(double);
    void setSpeed(int);
    void setahead();
    void setback();
    void setleft();
    void setright();
    void stop();
    void ahead();
    void ahead_ultra();//borrar
    float nearWall();//ver
    int findNearest(int,int[],int);
    void back();
    void left();
    void right();
    void showSpeeds();
    float calculateAngularDistance();
    void rotate(float);
    float changeSpeedMove(bool,bool,int);
    void resetTics();
    double getAvergeTics();
    void printAngle();
    void setupVlx(const int);
    void printTicsSpeed();
};
#endif
