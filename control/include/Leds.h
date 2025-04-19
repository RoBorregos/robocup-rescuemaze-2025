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
    uint32_t kYellowBite; //Blue
    uint8_t bright=40;
    static constexpr char kWhiteColor='W';
    Adafruit_NeoPixel strip=Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
public:
    Leds();
    void setupLeds();
    void setBrightness(uint8_t);
    void setColor(uint32_t);
    void sequency();
    void turnOff();
    void setRed();
    void setYellow();
    void setGreen();
    void setWhite();
    void harmedVictim();
    void stableVictim();
    void unharmedVictim();
};
#endif 
