#ifndef MOTORS_H
#define MOTORS_H
#include <Arduino.h>
#include "motor.h"
#include "PID.H"
#include "VLX.h"
#include "BNO.H"
#include <SPI.h>
#include <ESP32Servo.h>
#include "TCS.h"
#include "LimitSwitch.h"
#include "Leds.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define USING_SCREEN 1

#define PCA9548A_ADDR 0x70   // Dirección del PCA9548A
#define PCA9548A_CHANNEL_4 0x20  // Canal 4 (SDA4/SCL4)

constexpr uint8_t edgeTileDistance=6;
constexpr uint8_t kTileLength=30;
constexpr uint8_t rulet[4][4]={{0,1,2,3},{3,0,1,2},{2,3,0,1},{1,2,3,0}};
constexpr uint8_t targetDistances[]={edgeTileDistance+2,kTileLength+edgeTileDistance+2};
// constexpr uint8_t targetDistances[]={edgeTileDistance+5,kTileLength+edgeTileDistance+4};
constexpr uint8_t targetDistancesB[]={kTileLength+edgeTileDistance-2,2*kTileLength+edgeTileDistance-2};
// constexpr uint8_t targetDistancesB[]={kTileLength+edgeTileDistance,2*kTileLength+edgeTileDistance-4,};


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
    static constexpr uint8_t kNumVlx=8;
    static constexpr uint8_t maxVlxDistance=68;
    static constexpr uint8_t brakingDis=2;
    static constexpr uint8_t kDistanceToWall=15;
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
    static constexpr uint16_t kMinSpeedRotate=7;////////////////
    static constexpr uint16_t kMaxSpeedRotate=30;
    static constexpr uint16_t kMinSpeedFormard=5;//36
    static constexpr uint16_t kMaxSpeedFormard=30;//70
    static constexpr uint16_t kSpeedRampUp=20;//70
    static constexpr uint16_t kSpeedRampDown=9;//70
    //ramp
    PID rampUpPID;
    PID rampDownPID;
    static constexpr float kMinRampOrientation=18.0;
    static constexpr float minDisToLateralWall=6;
    static constexpr float impactDisToLateralWall=2;
    float changeAngle=0;

    //control Walls
    static constexpr uint8_t maxChangeAngle=3;
    //PID movement constants
    static constexpr float kP_mov=1;
    static constexpr float kI_mov=0.00;
    static constexpr float kD_mov=1.35;
    static constexpr uint8_t movTime=20;
    //ramp down constants
    static constexpr float kP_RampDown=0.15;
    static constexpr float kI_RampDown=0.00;
    static constexpr float kD_RampDown=0.03;
    static constexpr uint8_t rampTime=20;
    //ramp Up constants
    static constexpr float kP_RampUp=0.2;
    static constexpr float kI_RampUp=0.01;
    static constexpr float kD_RampUp=0.1;
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
    bool limitColition=false;
    //servo
    float servoPos=90;
    static constexpr uint16_t servoPosRight=136;
    static constexpr uint16_t servoPosLeft=47;
public:
    //objets
    Adafruit_VL53L0X lox = Adafruit_VL53L0X();
    BNO bno;
    TCS tcs_;
    LimitSwitch limitSwitch_[2];
    VLX vlx[kNumVlx];
    Servo servo;
    Motor motor[4];
    Leds leds;
    //public variables
    unsigned long buttonTime=millis();
    bool inMotion=false;
    bool blackTile=false;
    bool blueTile=false;
    bool checkpoint=false;
    uint8_t victim=0;
    uint8_t kitState=kitID::kNone;
    bool buttonPressed=false;
    uint8_t rampState=rampID::kNone;
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
    void setleftTraslation();
    void setrightTraslation();
    void stop();
    void calibrateColors();
    //movements
    void ahead();    
    void back();
    void left();
    void right();  
    void rotate(float);  
    void moveDistance(uint8_t targetDistance,bool);
    //setups
    void setupTCS();
    void setupVlx(const uint8_t);
    void resetOrientation();
    //sensors
    void checkTileColor();
    void limitCrash();
    float nearWall();
    void passObstacle();
    bool isWall(uint8_t);
    //victims
    void harmedVictim();
    void stableVictim();
    void unharmedVictim();
    void kitRight(uint8_t);
    void kitLeft(uint8_t);
    //gets
    float getCurrentDistanceCm();
    float getAngleOrientation();
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
