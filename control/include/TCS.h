#ifndef TCS_h
#define TCS_h
#include <FastLED.h>
#include "Adafruit_TCS34725.h"
#include "MUX.h"
// #include "CustomSerial.h"
#include <Adafruit_ADS1X15.h>
#include <Wire.h>
#include "ColorConverterLib.h"

// TODO: check which address is the correct one
#define TCS_ADDR 0x30 
constexpr uint8_t blackThreshold=40;

class TCS {
    private:
        Adafruit_TCS34725 tcs_ = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
        MUX mux_;
        Adafruit_ADS1115 photoresistor;

        static constexpr int kColorAmount_ = 3;


        int8_t colorAmount_ = 3;
        int8_t precision_;
        const char *colorList_;
        // float red_;
        // float green_;
        // float blue_;
        const int16_t (*colors_)[kColorAmount_];

        static constexpr int8_t kColorThresholdsAmount_ = 6;
        const int16_t (*colorThresholds_)[kColorThresholdsAmount_];

        static constexpr int8_t kColumnAmount_ = 3;


        static constexpr int8_t kPrecision_ = 10;

        static constexpr int8_t millisToWait_ = 5;
        //BLUE TILE
        static constexpr float kRedValueInBlue_ = 161.00;
        static constexpr float kGreenValueInBlue_ = 232.00;
        static constexpr float kBlueValueInBlue_ = 340.00;
        //BLACK TILE
        static constexpr float kRedValueInBlack_ = 94.00;
        static constexpr float kGreenValueInBlack_ = 56.00;
        static constexpr float kBlueValueInBlack_ = 45.00;
        //CHECKPOINT TILE
        static constexpr float kRedValueInCheckpoint_ = 1540.00;
        static constexpr float kGreenValueInCheckpoint_ = 1354.00;
        static constexpr float kBlueValueInCheckpoint_ = 1272.00;

        static constexpr uint8_t rgbThreshold=50;
        static constexpr float kMinRedValueInBlue_ = kRedValueInBlue_-rgbThreshold;
        static constexpr float kMaxRedValueInBlue_ = kRedValueInBlue_+rgbThreshold;

        static constexpr float kMinGreenValueInBlue_ = kGreenValueInBlue_-rgbThreshold;
        static constexpr float kMaxGreenValueInBlue_ = kGreenValueInBlue_+rgbThreshold;

        static constexpr float kMinBlueValueInBlue_ = kBlueValueInBlue_-rgbThreshold;
        static constexpr float kMaxBlueValueInBlue_ = kBlueValueInBlue_+rgbThreshold;

        static constexpr float kMaxRedValueInRed_ = 0;
        static constexpr float kMinRedValueInRed_ = 0;

        static constexpr float kMaxGreenValueInRed_ = 0;
        static constexpr float kMinGreenValueInRed_ = 0;

        static constexpr float kMaxBlueValueInRed_ = 0;
        static constexpr float kMinBlueValueInRed_ = 0;

        static constexpr float kMinRedValueInBlack_ = kRedValueInBlack_-rgbThreshold;
        static constexpr float kMaxRedValueInBlack_ = kRedValueInBlack_+rgbThreshold;

        static constexpr float kMinGreenValueInBlack_ = kGreenValueInBlack_-rgbThreshold;
        static constexpr float kMaxGreenValueInBlack_ = kGreenValueInBlack_+rgbThreshold;

        static constexpr float kMinBlueValueInBlack_ = kBlueValueInBlack_-rgbThreshold;
        static constexpr float kMaxBlueValueInBlack_ = kBlueValueInBlack_+rgbThreshold;

        static constexpr float kMinRedValueInCheckpoint_ = kRedValueInCheckpoint_-rgbThreshold;
        static constexpr float kMaxRedValueInCheckpoint_ = kRedValueInCheckpoint_+rgbThreshold;

        static constexpr float kMinGreenValueInCheckpoint_ = kGreenValueInCheckpoint_-rgbThreshold;
        static constexpr float kMaxGreenValueInCheckpoint_ = kGreenValueInCheckpoint_+rgbThreshold;

        static constexpr float kMinBlueValueInCheckpoint_ = kBlueValueInCheckpoint_-rgbThreshold;
        static constexpr float kMaxBlueValueInCheckpoint_ = kBlueValueInCheckpoint_+rgbThreshold;

        static constexpr char kRedColor_ = 'r';
        static constexpr char kBlueColor_ = 'B';
        static constexpr char kBlackColor_ = 'N';
        static constexpr char kCheckpointColor_ = 'C';
        static constexpr char kUndefinedColor_ = 'U';

        float kMinPhotoresistorValue_ = 11544;
        float kMaxPhotoresistorValue_ = 12544;
        static constexpr float kPhotoresistorThreshold_ = 500;

        bool inRange(const uint8_t color, const uint8_t colorRegistered);

        bool inRangeThreshold(const double lowerBound, const double colorDetection, const double upperBound);

        void setDefaultValues();

        float kRangeTolerance_ = 50;

    public:
        float red_;
        float green_;
        float blue_;
        float clear_;
        TCS();

        TCS(const uint8_t posMux);

        TCS(const uint8_t posMux, const int precision);
        
        void init();

        void init(const int16_t colors[][kColumnAmount_], const int8_t colorAmount, const char colorList[], const int16_t colorThresholds[][kColorThresholdsAmount_]);

        void setMux(const uint8_t posMux);

        void setPrecision(const uint8_t precision);

        void printRGB();
        void HSV();
        void printRGBC();

        void printColor();

        void updateRGB();

        void updateRGBC();

        char getColor();

        char getColorClear(){return 'r';};

        char getColorWithPrecision();
        
        char getColorWithThresholds();

        char getColorMode(const int sampleSize, const double certainity = 0);

        char getColorKReps(const int reps);

        void printColorMatrix();

        void printColorList();

        void getBlueRanges();

        void getBlackRanges();

        void getCheckpointRanges();
};

#endif