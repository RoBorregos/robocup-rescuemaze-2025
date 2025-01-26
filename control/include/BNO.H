#ifndef BNO_h
#define BNO_h

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <SPI.h>
extern float z_rotation;//angulo de rotacion
extern float angle;
class BNO {
    private:
        Adafruit_BNO055 bno_;
        sensors_event_t event_;
        double phaseCorrection_ = 0.0;
        double phaseCorrectionY_=0.0;
    public:
        // TODO: Add more comments on how it works
        double angle_initial;
        BNO();
        void setupBNO();
        void updateBNO(sensors_event_t &event);
        double getOrientationX();
        double getOrientationY();
        void setPhaseCorrection(const double phaseCorrection);
        void setPhaseCorrectionY(const double phaseCorrectionY);
        void resetOrientation();
};

#endif