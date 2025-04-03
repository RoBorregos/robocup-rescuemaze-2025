#include "Leds.h"

Leds::Leds():strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800){
}
void Leds::setupLeds(){
    strip.begin();
    kWhiteBite=strip.Color(   255,   255, 255); // True white (not RGB white)
    kRedBite=strip.Color(255,   0,   0); //Red
    kGreenBite=strip.Color(  0, 255,   0); // Green;
    kBlueBite=strip.Color(  0,   0, 255); //Blue
    strip.setBrightness(40);
    strip.fill(strip.Color(255, 255, 255)); // White color
    strip.show();
}
void Leds::setBrightness(uint8_t brightness){
    strip.setBrightness(brightness);
}
void Leds::setColor(char color){
    strip.setBrightness(20);
    if(color=kWhiteColor){
        strip.fill(strip.Color(255, 255, 255)); // White color
        strip.show();
    }
}
void Leds::turnOff(){
    strip.setBrightness(0);
    strip.fill(strip.Color(0, 0, 0)); // White color
    strip.show();
}

void Leds::sequency(){
    turnOff();
    setBrightness(20);
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
        strip.setPixelColor(i,kRedBite);         //  Set pixel's color (in RAM)
        strip.show();                          //  Update strip to match
        delay(500);                           //  Pause for a moment
      }
}

