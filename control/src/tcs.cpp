#include "TCS.h"

#define DEBUG_TCS 0
TCS::TCS() {
    setDefaultValues();
}

TCS::TCS(const uint8_t posMux) {
    mux_.setNewChannel(posMux);
    setDefaultValues();
}

void TCS::init() {
    mux_.selectChannel();
    if (!tcs_.begin()) {
        while(1){
            #if DEBUG_TCS
            customPrintln("No TCS34725 found ... check your connections");
            #endif
        }
        
    }Serial.println("TCS inicializado");
}

void TCS::setDefaultValues() {
    red_ = 0;
    green_ = 0;
    blue_ = 0;
}

void TCS::updateRGB() {
    mux_.selectChannel();
    tcs_.setInterrupt(false);
    delay(millisToWait_);
    tcs_.getRGB(&red_, &green_, &blue_);
    tcs_.setInterrupt(true);
}

void TCS::updateRGBC() {
    uint16_t redR;
    uint16_t greenR;
    uint16_t blueR;
    uint16_t clearR;
    mux_.selectChannel();
    tcs_.setInterrupt(false);
    delay(millisToWait_);
    tcs_.getRawData(&redR, &greenR, &blueR, &clearR);
    red_ = redR;
    green_ = greenR;
    blue_ = blueR;
    clear_= clearR;
    tcs_.setInterrupt(true);
}

void TCS::printRGB() {
    // updateRGBC();
    Serial.println(red_);
    Serial.println(green_);
    Serial.println(blue_);
}

void TCS::printRGBC() {
    const unsigned long t = millis();
    updateRGBC();
    #if DEBUG_TCS
    customPrint("Time:\t"); customPrintln(millis() - t);
    customPrint("R:\t"); customPrintln(red_);
    customPrint("G:\t"); customPrintln(green_);
    customPrint("B:\t"); customPrintln(blue_);
    #endif
}

void TCS::setMux(const uint8_t posMux) {
    mux_.setNewChannel(posMux);
}


char TCS::getColor() {
    updateRGBC();
    char colorLetter = kUndefinedColor_;

    // TODO: check each color
    //Serial.println("red: " + String(red_) + " green: " + String(green_) + " blue: " + String(blue_) + "clear" + String(clear_));
    // Serial.println("for BLUE: max red: " + String(kMaxRedValueInBlue_) + " min green: " + String(kMinGreenValueInBlue_) + " min blue: " + String(kMinBlueValueInBlue_));
    // Serial.println("for RED: min red: " + String(kMinRedValueInRed_) + " max green: " + String(kMaxGreenValueInRed_) + " max blue: " + String(kMaxBlueValueInRed_));
    // Serial.println("for BLACK: max red: " + String(kMaxRedValueInBlack_) + " max green: " + String(kMaxGreenValueInBlack_) + " max blue: " + String(kMaxBlueValueInBlack_));
    // customPrint(String(kMinRedValueInBlue_) + " " + String(kMaxRedValueInBlue_) + " " + String(kMinGreenValueInBlue_) + " " + String(kMaxGreenValueInBlue_) + " " + String(kMinBlueValueInBlue_) + " " + String(kMaxBlueValueInBlue_) + "\n");
    // customPrint(String(kMinRedValueInBlack_) + " " + String(kMaxRedValueInBlack_) + " " + String(kMinGreenValueInBlack_) + " " + String(kMaxGreenValueInBlack_) + " " + String(kMinBlueValueInBlack_) + " " + String(kMaxBlueValueInBlack_) + "\n");
    if (red_ > kMinRedValueInBlue_ && green_ > kMinGreenValueInBlue_ && blue_ > kMinBlueValueInBlue_ && red_ < kMaxRedValueInBlue_  && green_ < kMaxGreenValueInBlue_ && blue_ < kMaxBlueValueInBlue_) {
        // blue
        colorLetter = kBlueColor_;
        #if DEBUG_TCS
        customPrintln("blue");
        #endif
    } else if (red_ > kMinRedValueInBlack_ && green_ > kMinGreenValueInBlack_ && blue_ > kMinBlueValueInBlack_ && red_ < kMaxRedValueInBlack_ && green_ < kMaxGreenValueInBlack_ && blue_ < kMaxBlueValueInBlack_) {
        // black
        colorLetter = kBlackColor_;
        #if DEBUG_TCS
        customPrintln("black");
        #endif
    } else if (red_ > kMinRedValueInCheckpoint_ && green_ > kMinGreenValueInCheckpoint_ && blue_ > kMinBlueValueInCheckpoint_ && red_ < kMaxRedValueInCheckpoint_ && green_ < kMaxGreenValueInCheckpoint_ && blue_ < kMaxBlueValueInCheckpoint_) { // adc < kMinPhotoresistorValue_ || adc > kMaxPhotoresistorValue_
        colorLetter = kCheckpointColor_;
        #if DEBUG_TCS
        customPrintln("checkpoint");
        #endif
    } else {
        colorLetter = kUndefinedColor_;
        #if DEBUG_TCS
        customPrintln("unknown");
        #endif
    }
    #if DEBUG_TCS
    customPrint("colorLetter: "); customPrintln(colorLetter);
    #endif
    return colorLetter;
}

