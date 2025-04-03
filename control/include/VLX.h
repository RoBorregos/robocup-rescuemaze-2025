#ifndef VLX_H
#define VLX_H
#include "Pins_ID.h"
#include <Adafruit_VL53L0X.h>
#include <Adafruit_Sensor.h>
#include "SingleEMAFilter.h"
#include <Wire.h>
#include <Arduino.h>
#include "MUX.H"

class VLX{
private:
    MUX mux_;
    
    static constexpr uint8_t kMaxInitAttempts_ = 5;
public:
    static constexpr uint8_t kDistanceToWall=15;
    Adafruit_VL53L0X VLX_ = Adafruit_VL53L0X();
    VL53L0X_RangingMeasurementData_t measure;
    float distance;
    VLX();
    VLX(const uint8_t);
    void setMux(const uint8_t);
    void begin();
    VLX(int,int);
    void updateDistance();
    float getDistance();
    void printDistance();
    bool isWall();

};
#endif

