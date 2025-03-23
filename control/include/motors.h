#ifndef MOTORS_H
#define MOTORS_H
#include <Arduino.h>
#include "motor.h"
#include "PID.H"
#include "VLX.h"
#include "BNO.H"
#include <SPI.h>
#include <ESP32Servo.h>
#include "Screen.h"
#include "TCS.h"
#include "LimitSwitch.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define USING_SCREEN 1

#define PCA9548A_ADDR 0x70   // Dirección del PCA9548A
#define PCA9548A_CHANNEL_4 0x20  // Canal 4 (SDA4/SCL4)
constexpr uint8_t edgeTileDistance=7;
constexpr uint8_t kTileLength=30;
constexpr uint8_t rulet[4][4]={{0,1,2,3},{3,0,1,2},{2,3,0,1},{1,2,3,0}};
constexpr uint8_t targetDistances[]={edgeTileDistance,kTileLength+edgeTileDistance,2*kTileLength+edgeTileDistance,3*kTileLength+edgeTileDistance};
struct Advanced{
    int x,y;
};
class motors{
private:
    // static constexpr uint8_t edgeTileDistance=8;
    float targetAngle=0;
    static constexpr unsigned long delayTime=350;
    PID myPID[4];
    //vlx
    static constexpr uint8_t kNumVlx=6;
    static constexpr uint8_t maxVlxDistance=68;
    //wheels
    static constexpr float wheelDiameter=8.5;
    static constexpr float distancePerRev=wheelDiameter*PI;
    static constexpr float kTicsPerRev=496.0;
    static constexpr float kTicsPerTile=30*kTicsPerRev/distancePerRev;
    //Pwm constants
    uint16_t kMinPwmRotate=70;
    uint16_t kMaxPwmRotate=160;
    uint16_t kMinPwmFormard=70;
    uint16_t kMaxPwmFormard=180;
    //Speeds constants
    static constexpr uint16_t kMinSpeedRotate=8;////////////////
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
    //servo
    float servoPos=0;
public:
    //objets
    Adafruit_VL53L0X lox = Adafruit_VL53L0X();
    BNO bno;
    TCS tcs_;
    LimitSwitch limitSwitch_[2];
    VLX vlx[kNumVlx];
    Servo servo;
    Motor motor[4];
    //public variables
    bool blackTile=false;
    bool blueTile=false;
    bool checkpoint=false;
    bool victim=false;
    bool buttonPressed=false;
    uint8_t rampState;
    motors();//constructor
    //PID´s--speeds
    void setupMotors();
    void PID_speed(float, float, uint16_t);
    void PID_Wheel(int,int);
    void PID_AllWheels(int);
    void pidEncoders(int,bool);
    void setSpeed(uint16_t);
    float changeSpeedMove(bool,bool,int,bool);
    //set movement
    void setahead();
    void setback();
    void setleft();
    void setright();
    void stop();
    //movements
    void ahead();    
    void back();
    void left();
    void right();  
    void rotate(float);  
    void moveDistance(uint8_t targetDistance);
    //setups
    void setupTCS();
    void setupVlx(const uint8_t);
    //sensors
    void checkTileColor();
    float limitCrash();
    float nearWall();
    float passObstacle();
    bool isWall(uint8_t);
    //victims
    void harmedVictim();
    void stableVictim();
    void unharmedVictim();
    //gets
    float getCurrentDistanceCm();
    uint16_t getAngleOrientation();
    double getAvergeTics();
    double getTicsSpeed();
    //logic
    Advanced checkpointElection();
    uint8_t findNearest(float,const uint8_t[],uint8_t,bool);
    float calculateAngularDistance();
    void resetTics();
    //ramp
    bool rampInFront();
    bool isRamp();
    void ramp();
    //comunication
    void wait(unsigned long);
    void wifiPrint(String,float);
    void screenBegin();
    void screenPrint(String);
    void printSpeeds();
    void printAngle();
};

#endif
