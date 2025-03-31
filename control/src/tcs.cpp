#include "TCS.h"

#define DEBUG_TCS 0
TCS::TCS() {
    setDefaultValues();
}

TCS::TCS(const uint8_t posMux) {
    mux_.setNewChannel(posMux);
    setDefaultValues();
}

TCS::TCS(const uint8_t posMux, const int precision) {
    mux_.setNewChannel(posMux);
    setDefaultValues();
    this->precision_ = precision;
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
    photoresistor.begin();
}

void TCS::init(const int16_t colors[][3], const int8_t colorAmount, const char colorList[], const int16_t colorThresholds[][6]) {
    this->colorThresholds_ = colorThresholds;
    this->colorList_ = colorList;
    this->colors_ = colors;
    this->colorAmount_ = colorAmount;
    init();
}

void TCS::setDefaultValues() {
    red_ = 0;
    green_ = 0;
    blue_ = 0;

    const char tempColorList[4] = {"rnb"};
    colorList_ = tempColorList;
    colors_ = nullptr;
    colorThresholds_ = nullptr;
    precision_ = kPrecision_;
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
    updateRGBC();
    Serial.println(red_);
    Serial.println(green_);
    Serial.println(blue_);
}
// void TCS::HSV(){
//     updateRGB();
//     //  CRGB colorRGB = CRGB(205, 183, 155);//blanco
//     CRGB colorRGB = CRGB(red_, green_, blue_);//blue
//     // Convertir a HSV
//     CHSV colorHSV = rgb2hsv_approximate(colorRGB);

//     if(colorHSV.h>=blueLow[0] && colorHSV.h<=blueHigh[0] && colorHSV.s>=blueLow[1] && colorHSV.s<=blueHigh[1] && colorHSV.v>=blueLow[2] && colorHSV.v<=blueHigh[2]){
//         Serial.println("blue");
//     }else if(colorHSV.h>=blackLow[0] && colorHSV.h<=blackHigh[0] && colorHSV.s>=blackLow[1] && colorHSV.s<=blackHigh[1] && colorHSV.v>=blackLow[2] && colorHSV.v<=blackHigh[2]){
//         Serial.println("black");
//     }else if(colorHSV.h>=greyLow[0] && colorHSV.h<=greyHigh[0] && colorHSV.s>=greyLow[1] && colorHSV.s<=greyHigh[1] && colorHSV.v>=greyLow[2] && colorHSV.v<=greyHigh[2]){
//         Serial.println("grey");
//     }else{
//         Serial.println("undef");
//     }
//     Serial.print("H: "); Serial.print(colorHSV.h);
//     Serial.print(" S: "); Serial.print(colorHSV.s);
//     Serial.print(" V: "); Serial.println(colorHSV.v);
// }
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

void TCS::setPrecision(const uint8_t precision) {
    this->precision_ = precision;
}

char TCS::getColor(){
    double nuanceT, saturationT, valueT;
    updateRGBC();
    float scaleRedT = red_;
    float scaleGreenT = green_;
    float scaleBlueT = blue_;
    if(clear_>0){
        scaleRedT = (scaleRedT / clear_ ) * 255;
        scaleGreenT = (scaleGreenT / clear_ ) * 255;
        scaleBlueT = (scaleBlueT / clear_ ) * 255;
    }
    ColorConverter::RgbToHsv(static_cast<uint16_t>(scaleRedT), static_cast<uint16_t>(scaleGreenT),
                            static_cast<uint16_t>(scaleBlueT), nuanceT, saturationT, valueT);
    nuanceT *= 360;
    Serial.println("HSU");
    Serial.println(nuanceT);
    Serial.println(saturationT);
    Serial.println(valueT);

    Serial.println("RGB");
    Serial.println(red_);
    Serial.println(green_);
    Serial.println(blue_); 
    Serial.println(clear_); 


    if(nuanceT>150 && nuanceT<280) return kBlueColor_;
    
    else if(red_<blackThreshold && green_<blackThreshold && blue_<blackThreshold) return kBlackColor_;
    // else if(nuanceT<20) return kRedColor_;
    else return kUndefinedColor_;

  
}
bool TCS::inRange(uint8_t colorInput, uint8_t colorRegistered) {
    return (((colorRegistered - precision_) <= colorInput) && (colorInput <= (colorRegistered + precision_)));
}

bool TCS::inRangeThreshold(double lowerBound, double colorDetection, double upperBound) {
    if (lowerBound > upperBound) {
        const double temp = lowerBound;
        lowerBound = upperBound;
        upperBound = temp;
    }

    return (lowerBound <= colorDetection) && (colorDetection <= upperBound);
}

char TCS::getColorWithPrecision() {
    if (colors_ == nullptr) {
        return getColor();
    }

    updateRGBC();
    #if DEBUG_TCS
    customPrint("R:\t"); customPrintln(red_);
    customPrint("G:\t"); customPrintln(green_);
    customPrint("B:\t"); customPrintln(blue_);
    customPrint("Precision:\t"); customPrintln(precision_);
    #endif

    for (uint8_t i = 0; i < colorAmount_; ++i) {
        if (inRange(red_, colors_[i][0]) &&
            inRange(green_, colors_[i][1]) &&
            inRange(blue_, colors_[i][2])) {

            return colorList_[i];
        }
    }

    return kUndefinedColor_;
}

char TCS::getColorWithThresholds() {
    if (colorThresholds_ == nullptr) {
        return getColorWithPrecision();
    }
    updateRGBC();
    // customPrint("R:\t"); customPrintln(red_);
    // customPrint("G:\t"); customPrintln(green_);
    // customPrint("B:\t"); customPrintln(blue_);

    for (uint8_t i = 0; i < colorAmount_; ++i) {
        if (inRangeThreshold(colorThresholds_[i][0], red_, colorThresholds_[i][1]) &&
            inRangeThreshold(colorThresholds_[i][2], green_, colorThresholds_[i][3]) &&
            inRangeThreshold(colorThresholds_[i][4], blue_, colorThresholds_[i][5])) {

            return colorList_[i];
        }
    }


    return kUndefinedColor_;
}

char TCS::getColorKReps(const int reps) {
    const char start = getColorWithPrecision();

    if (start == kUndefinedColor_) {
        return start;
    }

    for (uint8_t i = 0; i < reps; ++i) {
        const char current = getColorWithPrecision();
        if (current != start) {
            return kUndefinedColor_;
        }
    }

    return start;
}

char TCS::getColorMode(const int sampleSize, const double certainity) {
    uint8_t mode = 0;
    uint16_t unknown = sampleSize;
    uint8_t repetitions[colorAmount_];

    for (uint8_t i = 0; i < colorAmount_; ++i) {
        repetitions[i] = 0;
    }

    for (uint8_t i = 0; i < sampleSize; ++i) {
        const char current = getColorWithPrecision();
        if (current == kUndefinedColor_) {
            return kUndefinedColor_;
        }

        for (uint8_t j = 0; j < colorAmount_; ++j) {
            if (current == colorList_[j]) {
                repetitions[j]++;
                break;
            }
        }
    }

    for (uint8_t i = 0; i < colorAmount_; ++i) {
        if (repetitions[mode] < repetitions[i]) {
            mode = i;
        }
        unknown -= repetitions[i];
    }
    const double probability = (repetitions[mode]) / (double)sampleSize;

    if (repetitions[mode] > unknown && probability > certainity) {
        return colorList_[mode];
    }
    return kUndefinedColor_;
}

void TCS::printColorMatrix() {
    #if DEBUG_TCS
    if (colors_ == nullptr) {
        customPrintln("No colors registered");
        return;
    }

    for (uint8_t i = 0; i < colorAmount_; ++i) {
        customPrint(colorList_[i]);
        customPrint(":\t");
        customPrint(colors_[i][0]);
        customPrint("\t");
        customPrint(colors_[i][1]);
        customPrint("\t");
        customPrintln(colors_[i][2]);
    }
    #endif
}

void TCS::printColorList() {
#if DEBUG_TCS
    if (colorList_ == nullptr) {
        customPrintln("No colors registered");
        return;
    }

    for (uint8_t i = 0; i < colorAmount_; ++i) {
        customPrint(colorList_[i]);
        customPrint("\t");
    }

    customPrintln("");
#endif
}
