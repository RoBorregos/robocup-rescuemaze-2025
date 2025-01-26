#ifndef VLX_H
#define VLX_H
#include "Pins.h"
#include <Adafruit_VL53L0X.h>
#include <Wire.h>
#include <Arduino.h>
#include "MUX.H"

namespace vlxID{
    constexpr uint8_t frontLeft=0;
    constexpr uint8_t left=1;
    constexpr uint8_t back=2;
    constexpr uint8_t Right=3;
    constexpr uint8_t frontRight=4;
}
class VLX{
private:
    MUX mux_;
    const int kDistanceToWall=15;
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

