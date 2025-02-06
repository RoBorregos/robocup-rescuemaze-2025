#ifndef VLX_H
#define VLX_H
#include "Pins.h"
#include <Adafruit_VL53L0X.h>
#include <Adafruit_Sensor.h>
#include "SingleEMAFilter.h"
#include <Wire.h>
#include <Arduino.h>
#include "MUX.H"

namespace vlxID{
    constexpr uint8_t right=2;//
    constexpr uint8_t left=4;////////////////////////
    constexpr uint8_t frontRight=5;//
    constexpr uint8_t frontLeft=3;//
    //constexpr uint8_t front=4;
    constexpr uint8_t back=0;//
}
//canels del mux (provisonales)
  //canal 3 front left 0x08
  //canal 5 front right 0x20
  //0x02 color
  //0x04 right
class VLX{
private:
    MUX mux_;
    const int kDistanceToWall=15;
    static constexpr double kMaxInitAttempts_ = 5;
public:
    Adafruit_VL53L0X VLX_ = Adafruit_VL53L0X();
    VL53L0X_RangingMeasurementData_t measure;
    double distance;
    VLX();
    VLX(const uint8_t);
    void setMux(const uint8_t);
    void begin();
    VLX(int,int);
    void updateDistance();
    double getDistance();
    void printDistance();
    bool isWall();

};
#endif

