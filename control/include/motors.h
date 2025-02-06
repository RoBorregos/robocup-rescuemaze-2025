#ifndef MOTORS_H
#define MOTORS_H
#include <Arduino.h>
#include "motor.h"
#include "PID.H"
#include "VLX.h"
#include "BNO.H"
#include <SPI.h>
#define PCA9548A_ADDR 0x70   // Dirección del PCA9548A
#define PCA9548A_CHANNEL_4 0x20  // Canal 4 (SDA4/SCL4)
// #include <math.h>
class motors{
private:
    const int kTileLength=30;
    int edgeTileDistance=8.5;
    float targetAngle=0;
    unsigned long delayTime=350;
    PID myPID[4];
    //vlx
    static constexpr int kNumVlx=5;
    double maxVlxDistance=120;
    //wheels
    const double wheelDiameter=8;
    const double distancePerRev=wheelDiameter*PI;
    const double kTicsPerRev=496.0;
    const double kTicsPerTile=30*kTicsPerRev/distancePerRev;
    //Pwm constants
    uint8_t kMinPwmRotate=70;
    uint8_t kMaxPwmRotate=160;
    uint8_t kMinPwmFormard=70;
    uint8_t kMaxPwmFormard=180;
    //Speeds constants
    const uint8_t kMinSpeedRotate=20;
    const uint8_t kMaxSpeedRotate=60;
    const uint8_t kMinSpeedFormard=20;//36
    const uint8_t kMaxSpeedFormard=65;//70
    //ramp
    double kMinRampOrientation=10.0;
    float minDisToLateralWall=3;
    float maxChangeAngle=10;
    float kMinAngleRamp=10;
public:
    Adafruit_VL53L0X lox = Adafruit_VL53L0X();
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
    double passObstacle();
    int findNearest(int,int[],int,bool);
    void back();
    void left();
    void right();
    void printSpeeds();
    float calculateAngularDistance();
    void rotate(float);
    float changeSpeedMove(bool,bool,int,bool);
    void changePwm(bool,double);
    void resetTics();
    double getAvergeTics();
    double getTicsSpeed();
    void printAngle();
    void setupVlx(const uint8_t);
    void printTicsSpeed();
    bool isWall(uint8_t);
    bool rampInFront();
    bool isRamp();
    void ramp();
    void wait(unsigned long);
    void wifiPrint(String,float);
};
#endif
