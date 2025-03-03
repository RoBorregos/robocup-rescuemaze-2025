#ifndef MOTORS_H
#define MOTORS_H
#include <Arduino.h>
#include "motor.h"
#include "PID.H"
#include "VLX.h"
#include "BNO.H"
#include <SPI.h>
#include <unordered_map>
#include "TCS.h"
#include "LimitSwitch.h"
#define PCA9548A_ADDR 0x70   // Dirección del PCA9548A
#define PCA9548A_CHANNEL_4 0x20  // Canal 4 (SDA4/SCL4)
constexpr uint8_t edgeTileDistance=5;
constexpr uint8_t rulet[4][4]={{0,1,2,3},{3,0,1,2},{2,3,0,1},{1,2,3,0}};
constexpr uint8_t targetDistances[]={edgeTileDistance,30+edgeTileDistance,60+edgeTileDistance,90+edgeTileDistance};
class motors{
private:
    static constexpr uint8_t kTileLength=30;
    // static constexpr uint8_t edgeTileDistance=8;
    float targetAngle=0;
    static constexpr unsigned long delayTime=350;
    PID myPID[4];
    //vlx
    static constexpr uint8_t kNumVlx=5;
    static constexpr uint8_t maxVlxDistance=68;
    
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
    static constexpr uint16_t kMinSpeedRotate=10;////////////////
    static constexpr uint16_t kMaxSpeedRotate=30;
    static constexpr uint16_t kMinSpeedFormard=10;//36
    static constexpr uint16_t kMaxSpeedFormard=30;//70
    static constexpr uint16_t kSpeedRampUp=20;//70
    static constexpr uint16_t kSpeedRampDown=15;//70


    //ramp
    static constexpr float kMinRampOrientation=10.0;
    static constexpr float minDisToLateralWall=6;
    static constexpr float impactDisToLateralWall=2;
    static constexpr uint8_t maxChangeAngle=5;
    static constexpr uint8_t kMinAngleRamp=10;
    //TCS
    char tileColor;
    static constexpr int kPrecision = 100;
    static constexpr uint8_t kColorAmount = 3;
    static constexpr uint8_t kColorThresholdsAmount = 6;
    const char kColorList[kColorAmount + 1] = {"RNB"};
    static constexpr char kBlueColor = 'B';
    static constexpr char kBlackColor = 'N';
    static constexpr char kRedColor = 'R';
    static constexpr char kCheckpointColor = 'C';
    static constexpr double kHorizontalAngleError = 10;
    const int16_t kColors[kColorAmount][kColorAmount] = {
        // RED
        {257, 75, 71},
        // BLACK
        {80, 44, 34},
        // BLUE
        {97,99,141}
    };
    
    const int16_t kColorThresholds[kColorAmount][kColorThresholdsAmount] {
        {220, 270, 60, 80, 50, 75},
        {20, 120, 30, 90, 20, 79},
        {85, 150, 80, 200, 120, 220}
    };
    //movement
    bool inMotion=false;
public:
    Adafruit_VL53L0X lox = Adafruit_VL53L0X();
    BNO bno;
    TCS tcs_;
    LimitSwitch limitSwitch_[2];
    VLX vlx[kNumVlx];
    Motor motor[4];//0-BACK_RIGHT//1-BACK_LEFT//2-FRONT_RIGHT//3-FRONT_LEFT
    bool blackTile=false;
    bool blueTile=false;
    bool checkpoint=false;
    bool victim=false;
    uint8_t rampState;
    motors();
    void setupMotors();
    void PID_speed(float, float, uint16_t);
    void PID_Wheel(int,int);
    void PID_AllWheels(int);
    void setSpeed(uint16_t);
    void setahead();
    void moveDistance(uint8_t targetDistance);
    float getCurrentDistanceCm();
    void setback();
    void setleft();
    void setright();
    void checkTileColor();
    void stop();
    void ahead();
    float limitCrash();
    uint16_t getAngleOrientation();
    void pidEncoders(int,bool);
    void ahead_ultra();//borrar
    float nearWall();//ver
    float passObstacle();
    uint8_t findNearest(float,const uint8_t[],uint8_t,bool);
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
    float getRealDistance();
    bool rampInFront();
    bool isRamp();
    void ramp();
    void setupTCS();
    void wait(unsigned long);
    void wifiPrint(String,float);
};

#endif
