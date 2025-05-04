#include "Leds.h"

Leds::Leds():strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800){
}
void Leds::setupLeds(){
    strip.begin();
    kWhiteBite=strip.Color(   255,   255, 255); // True white (not RGB white)
    kRedBite=strip.Color(255,   0,   0); //Red
    kGreenBite=strip.Color(  0, 255,   0); // Green;
    kBlueBite=strip.Color(  0,   0, 255); //Blue
    kYellowBite=strip.Color(  255,   255, 0); //Blue
    bright=40;
    strip.setBrightness(bright);
    strip.fill(strip.Color(255, 255, 255)); // White color
    strip.show();
    // turnOff();
}
void Leds::setBrightness(uint8_t brightness){
    bright=brightness;
    strip.setBrightness(brightness);
}
void Leds::setColor(uint32_t color){
    strip.setBrightness(bright);
    strip.fill(color); // White color
    strip.show();
}
void Leds::setRed(){
    setColor(kRedBite);
}
void Leds::setYellow(){
    setColor(kYellowBite);
}
void Leds::setGreen(){
    setColor(kGreenBite);
}
void Leds::setWhite(){
    setColor(kWhiteBite);
}
void Leds::turnOff(){
    strip.setBrightness(0);
    strip.fill(strip.Color(0, 0, 0)); // White color
    strip.show();
}

void Leds::harmedVictim(){
    float current=millis();
    while((millis()-current)<5100){
        setRed();
        delay(500);
        turnOff();
        delay(500);
    }
    setWhite(); 
    // turnOff();
}
void Leds::stableVictim(){
    float current=millis();
    while((millis()-current)<5100){
        setYellow();
        delay(500);
        turnOff();
        delay(500);
    }
    setWhite();
    // turnOff();
}
void Leds::unharmedVictim(){
    float current=millis();
    while((millis()-current)<5100){
        setGreen();
        delay(500);
        turnOff();
        delay(500);
    }
    setWhite();
    // turnOff();
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