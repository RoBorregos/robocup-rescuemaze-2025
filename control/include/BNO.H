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
        float phaseCorrection_ = 0.0;
        float phaseCorrectionY_=0.0;
    public:
        // TODO: Add more comments on how it works
        float angle_initial;
        BNO();
        void setupBNO();
        void updateBNO(sensors_event_t &event);
        float getOrientationX();
        float getOrientationY();
        void setPhaseCorrection(float phaseCorrection);
        void setPhaseCorrectionY(float phaseCorrectionY);
        void resetOrientation();
};

#endif