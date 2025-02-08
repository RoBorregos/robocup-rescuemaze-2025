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

constexpr uint8_t rulet[4][4]={{0,1,2,3},{3,0,1,2},{2,3,0,1},{1,2,3,0}};
class motors{
private:
    static constexpr uint8_t kTileLength=30;
    static constexpr uint8_t edgeTileDistance=8;
    float targetAngle=0;
    static constexpr unsigned long delayTime=350;
    // PID myPID[4];
    //vlx
    static constexpr uint8_t kNumVlx=5;
    static constexpr uint8_t maxVlxDistance=120;
    
    //wheels
    static constexpr float wheelDiameter=8;
    static constexpr float distancePerRev=wheelDiameter*PI;
    static constexpr float kTicsPerRev=496.0;
    static constexpr float kTicsPerTile=30*kTicsPerRev/distancePerRev;
    //Pwm constants
    uint16_t kMinPwmRotate=70;
    uint16_t kMaxPwmRotate=160;
    uint16_t kMinPwmFormard=70;
    uint16_t kMaxPwmFormard=180;
    //Speeds constants
    static constexpr uint16_t kMinSpeedRotate=15;////////////////
    static constexpr uint16_t kMaxSpeedRotate=60;
    static constexpr uint16_t kMinSpeedFormard=20;//36
    static constexpr uint16_t kMaxSpeedFormard=65;//70
    //ramp
    static constexpr float kMinRampOrientation=10.0;
    static constexpr float minDisToLateralWall=2;
    static constexpr float impactDisToLateralWall=2;
    static constexpr uint8_t maxChangeAngle=2;
    static constexpr uint8_t kMinAngleRamp=10;
public:
    Adafruit_VL53L0X lox = Adafruit_VL53L0X();
    BNO bno;
    VLX vlx[kNumVlx];
    motor_ motor[4];//0-BACK_RIGHT//1-BACK_LEFT//2-FRONT_RIGHT//3-FRONT_LEFT
    motors();
    void setupMotors();
    void PID_speed(float, float, uint16_t);
    void PID_encoders(double);
    void setSpeed(uint16_t);
    void setahead();
    void setback();
    void setleft();
    void setright();
    void stop();
    void ahead();
    void ahead_ultra();//borrar
    float nearWall();//ver
    double passObstacle();
    uint8_t findNearest(float,uint8_t[],uint8_t,bool);
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
