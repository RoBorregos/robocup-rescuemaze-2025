#ifndef LEDS_H
#define LEDS_H
#include <Adafruit_NeoPixel.h>
#include "Pins_ID.h"
#define PIN Pins::LedsPin
constexpr uint8_t numPixels=8;
#define NUMPIXELS numPixels


class Leds{
private:
    uint32_t kWhiteBite; // True white (not RGB white)
    uint32_t kRedBite; //Red
    uint32_t kGreenBite; // Green;
    uint32_t kBlueBite; //Blue
    static constexpr char kWhiteColor='W';
    Adafruit_NeoPixel strip=Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
public:
    Leds();
    void setupLeds();
    void setBrightness(uint8_t);
    void setColor(char);
    void sequency();
    void turnOff();

};
#endif 
